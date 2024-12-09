// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/BarrelRollAbility.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffects/ImmunityEffect.h"


UBarrelRollAbility::UBarrelRollAbility()
{
	ImmunityEffectClass = UImmunityEffect::StaticClass();

	CooldownDuration = 1.5f;
	CooldownTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ship.Action.Roll"), true));
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

			animInstance->Montage_Play(RollMontage, 1 , EMontagePlayReturnType::Duration, 0, true);
			animInstance->OnMontageEnded.AddDynamic(this, &UBarrelRollAbility::OnRollFinished);

			FVector impulseDir =ActorInfo->AvatarActor->GetVelocity().GetSafeNormal();
			skm->AddImpulse(impulseDir * RollImpulseStrength, NAME_None, true);
			return;
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

void UBarrelRollAbility::OnRollFinished(UAnimMontage* montage, bool bInterrupted)
{
	if(montage != RollMontage)return;
	
	CurrentActorInfo->SkeletalMeshComponent->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &UBarrelRollAbility::OnRollFinished);
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}


