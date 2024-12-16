// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/CooldownGameplayAbility.h"
#include "ShootGunAbility.generated.h"

/**
 * 
 */
UCLASS()
class UShootGunAbility : public UCooldownGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	float Damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	TSubclassOf<UGameplayEffect> OptionalGameplayEffectClass;
};
