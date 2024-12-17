// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/BarrelRollAbility.h"
#include "GameplayAbilities/Public/Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffects/ImmunityEffect.h"


UBarrelRollAbility::UBarrelRollAbility()
{
	ImmunityEffectClass = UImmunityEffect::StaticClass();

	CooldownDuration = 1.5f;
	CooldownTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effects.Cooldown.Roll"), true));

	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ship.Action.Roll"), true));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ship.Action.SwordSwing"), true));
	
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UBarrelRollAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const TWeakObjectPtr<USkeletalMeshComponent> skm = ActorInfo->SkeletalMeshComponent;
	if(skm != nullptr)
	{
		const TWeakObjectPtr<UAnimInstance> animInstance = skm->GetAnimInstance();

		if(animInstance != nullptr)
		{
			CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, nullptr);
			FGameplayEffectSpecHandle ImmunityEffect = MakeOutgoingGameplayEffectSpec(ImmunityEffectClass, GetAbilityLevel());
			ActiveGameplayEffect = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, ImmunityEffect);

			UAbilityTask_PlayMontageAndWait* playMontageAndWaitTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("Roll"), RollMontage, 1, NAME_None, true, 1, 0, false);

			if (playMontageAndWaitTask)
			{
				playMontageAndWaitTask->OnCompleted.AddDynamic(this, &UBarrelRollAbility::OnRollFinished);
				playMontageAndWaitTask->OnCancelled.AddDynamic(this, &UBarrelRollAbility::OnRollFinished);
				playMontageAndWaitTask->Activate();
				//animInstance->Montage_Play(RollMontage, 1 , EMontagePlayReturnType::Duration, 0, true);
				//	animInstance->OnMontageEnded.AddDynamic(this, &UBarrelRollAbility::OnRollFinished);

				FVector impulseDir =ActorInfo->AvatarActor->GetVelocity().GetSafeNormal();
				skm->AddImpulse(impulseDir * RollImpulseStrength, NAME_None, true);
				return;
			}
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UBarrelRollAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if(ActiveGameplayEffect.IsValid())
	{
		ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveGameplayEffect);	
	}
}

void UBarrelRollAbility::OnRollFinished()
{
	//PlayMontageAndWaitTask->OnCompleted.RemoveAll(this);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


