// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SwordSwingAbility.generated.h"

class UAnimMontage;
/**
 * 
 */
UCLASS()
class USwordSwingAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	USwordSwingAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> SwordSwingMontage;

private:
	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupted);


};
