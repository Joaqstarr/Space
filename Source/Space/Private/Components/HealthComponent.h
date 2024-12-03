// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UHealthSet;
struct FOnAttributeChangeData;
class UAbilitySystemComponent;

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

	UPROPERTY(BlueprintAssignable, Category = "Default")
	FOnHealthChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "Default")
	FOnHealthDepletedSignature OnHealthDepleted;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UHealthSet> HealthSet;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
private:
	UPROPERTY(VisibleInstanceOnly, Category="Default")
	float Health = 100.0f;
	UPROPERTY(VisibleAnywhere, Category="Default")
	float MaxHealth = 100;

	void HealthAttributeChanged(const FOnAttributeChangeData& data);

	void MaxHealthAttributeChanged(const FOnAttributeChangeData& data);

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
};
