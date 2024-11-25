// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDepletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, int, NewHealth, int, MaxHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();
	
	UFUNCTION(BlueprintCallable)
	int GetHealth() const {return Health;}

	UFUNCTION(BlueprintCallable)
	int GetMaxHealth() const {return MaxHealth;}
	
	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(int newMax);
	
	UFUNCTION(BlueprintCallable)
	void UpdateHealth(int DeltaHealth);

	UPROPERTY(BlueprintAssignable, Category = "Default")
	FOnHealthChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Default")
	FOnHealthDepletedSignature OnHealthDepleted;
protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
private:
	UPROPERTY(VisibleInstanceOnly, Category="Default")
	int Health = 100;
	UPROPERTY(EditDefaultsOnly, Category="Default")
	int MaxHealth = 100;
	UPROPERTY(EditAnywhere, Category="Default")
	float InvulnerabilityTime = 0;
	UPROPERTY()
	FTimerHandle IFramesTimer;

	bool IsInvulnerable = false;
};
