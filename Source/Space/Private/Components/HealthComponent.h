// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UHealthSet;
struct FOnAttributeChangeData;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthDepletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangedSignature, int, NewHealth, int, OldHealth, int, MaxHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHealthComponent : public UActorComponent, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();
	
	UFUNCTION(BlueprintCallable)
	int GetHealth() const;

	UFUNCTION(BlueprintCallable)
	int GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


public:
	/*
	 * Delegate that fires on health changed. Contains 3 int parameters, current health, old health, and max health
	 */
	UPROPERTY(BlueprintAssignable, Category = "Default")
	FOnHealthChangedSignature OnHealthChanged;
	/*
	 * Fires when health is zero, currently only on server.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Default")
	FOnHealthDepletedSignature OnHealthDepleted;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UHealthSet* HealthSet;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	int Health = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	int MaxHealth = 100;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:
	
	void HealthAttributeChanged(const FOnAttributeChangeData& data);
	void MaxHealthAttributeChanged(const FOnAttributeChangeData& data);
	
	void InitializeHealthValues();
	void CreateHealthAttributeSet();
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
};
