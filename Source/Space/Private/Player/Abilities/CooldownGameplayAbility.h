// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CooldownGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class UCooldownGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UCooldownGameplayAbility();
	
	virtual const FGameplayTagContainer* GetCooldownTags() const override;
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FScalableFloat CooldownDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
	FGameplayTagContainer CooldownTags;

private:
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle handle, const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo) const override;
	// Temp container that we will return the pointer to in GetCooldownTags().
	// This will be a union of our CooldownTags and the Cooldown GE's cooldown tags.
	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;
	
};
