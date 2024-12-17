// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/SwordSwingAbility.h"
#include "Actors/GASTargetActors/CapsuleTraceTargetActor.h"
#include "MotionWarpingComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Components/Player/PawnMotionWarpingComponent.h"
#include "Player/PlayerShip.h"

USwordSwingAbility::USwordSwingAbility()	
{
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ship.Action.Roll"), true));
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ship.Action.SwordSwing"), true));

	CooldownTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Effects.Cooldown.SwordSwing"), true));

	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USwordSwingAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	return;
	/*
	UAbilityTask_WaitTargetData* waitForTarget = UAbilityTask_WaitTargetData::WaitTargetData(this, FName(TEXT("WaitTargetData")), EGameplayTargetingConfirmation::UserConfirmed, CapsuleTargetClass);
	if(waitForTarget)
	{
		waitForTarget->ValidData.AddDynamic(this, &USwordSwingAbility::TargetDataReceived);	
		waitForTarget->Cancelled.AddDynamic(this, &USwordSwingAbility::TargetDataReceived);
		AGameplayAbilityTargetActor* spawnedActor;
		if(waitForTarget->BeginSpawningActor(this, CapsuleTargetClass, spawnedActor))
		{
			waitForTarget->FinishSpawningActor(this, spawnedActor);
			waitForTarget->ReadyForActivation();
			return;
		}

	}
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);*/
}

void USwordSwingAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USwordSwingAbility::MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CurrentActorInfo->GetAnimInstance()->OnMontageEnded.Remove(this, FName("MontageEnded"));
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void USwordSwingAbility::AddMotionWarpingTarget(FName warpTargetName, AActor* warpActor, APlayerShip* playerShip)
{
	//GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Red, TEXT("Add Motion Warping Target"));
	if(!playerShip)return;

	UPawnMotionWarpingComponent* motionWarpingComponent = playerShip->GetMotionWarping();
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Magenta, TEXT("WARPING COMPONENT"));

	if(!motionWarpingComponent)return;

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Magenta, warpActor->GetActorLocation().ToString());

	motionWarpingComponent->SetWarpTarget(warpActor);
}


void USwordSwingAbility::TargetDataReceived(const FGameplayAbilityTargetDataHandle& data)
{
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	UAnimInstance* animInstance = CurrentActorInfo->GetAnimInstance();
	if(animInstance && SwordSwingMontage)
	{
		FAlphaBlendArgs blendArgs(0.01f);
		animInstance->Montage_Play(SwordSwingMontage, 1, EMontagePlayReturnType::Duration);
		//animInstance->Montage_PlayWithBlendIn(SwordSwingMontage, blendArgs, 1, EMontagePlayReturnType::Duration, true);
		animInstance->OnMontageEnded.AddDynamic(this, &USwordSwingAbility::MontageEnded);
		return;
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}
