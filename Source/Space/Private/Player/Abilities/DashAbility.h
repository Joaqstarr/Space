// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DashAbility.generated.h"

/**
 * 
 */
UCLASS()
class UDashAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UDashAbility();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
protected:
	UFUNCTION(BlueprintCallable)
	void Dash(const FVector& inputDir);
private:
	void UpdateDirectionAndStrengthForTarget(FVector& dir, float& strength);
};
