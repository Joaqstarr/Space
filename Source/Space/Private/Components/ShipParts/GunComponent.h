// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GameplayTagContainer.h"
#include "GunComponent.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;
class UPoolManagerComponent;
class UTargetingHandlerComponent;
class ATargetable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGunComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UGunComponent();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerFire(ATargetable* Target);

	void SetDamage(float NewDamage) {Damage = NewDamage;};
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Weapon Stats")
	float Damage = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AbilityTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UPoolManagerComponent> ProjectilePool;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
	TObjectPtr<UTargetingHandlerComponent> TargetManager;

private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
