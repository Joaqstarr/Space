// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/SwordSwingAbility.h"

USwordSwingAbility::USwordSwingAbility()
{
}

void USwordSwingAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UAnimInstance* animInstance = ActorInfo->GetAnimInstance();
	if(animInstance && SwordSwingMontage)
	{
		FAlphaBlendArgs blendArgs(0.01f);
		animInstance->Montage_Play(SwordSwingMontage, 1, EMontagePlayReturnType::Duration);
		//animInstance->Montage_PlayWithBlendIn(SwordSwingMontage, blendArgs, 1, EMontagePlayReturnType::Duration, true);
		animInstance->OnMontageEnded.AddDynamic(this, &USwordSwingAbility::MontageEnded);
		return;
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
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
