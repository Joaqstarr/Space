// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Abilities/CooldownGameplayAbility.h"
#include "ShootGunAbility.generated.h"

struct FGameplayTag;

/**
 * 
 */
UCLASS()
class UShootGunAbility : public UCooldownGameplayAbility
{
	GENERATED_BODY()
	
public:
	UShootGunAbility();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	float Damage = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Stats")
	TSubclassOf<UGameplayEffect> OptionalProjectileEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TSubclassOf<UGameplayEffect> ShootEffect;
};
