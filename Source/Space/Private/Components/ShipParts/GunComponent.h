// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GunComponent.generated.h"


class UGameplayEffect;
class UAbilitySystemComponent;
class UTargetableComponent;
class UPoolManagerComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGunComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void Fire(ATargetable* lockedOnTarget);
	void SetDamage(float newDamage) {Damage = newDamage;};
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Weapon Stats")
	float Damage = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Weapon Stats")
	int32 RoundsPerMinute = 60;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Weapon Stats")
	TSubclassOf<UGameplayEffect> OptionalGameplayEffectClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UPoolManagerComponent> ProjectilePool;

	
private:
	float fireTimer = 0;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
