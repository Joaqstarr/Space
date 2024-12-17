// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/CooldownGameplayAbility.h"
#include "BarrelRollAbility.generated.h"

/**
 * 
 */
UCLASS()
class UBarrelRollAbility : public UCooldownGameplayAbility
{
	GENERATED_BODY()
public:
	UBarrelRollAbility();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float RollImpulseStrength = 1000;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UAnimMontage> RollMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> ImmunityEffectClass;
private:
	UFUNCTION()
	void OnRollFinished();

private:
	FActiveGameplayEffectHandle ActiveGameplayEffect;
};
