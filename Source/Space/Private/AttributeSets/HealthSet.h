// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SpaceAttributeSet.h"
#include "HealthSet.generated.h"



UCLASS()
class UHealthSet : public USpaceAttributeSet
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Health",  ReplicatedUsing= OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UHealthSet, Health)
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing= OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UHealthSet, MaxHealth)
	
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& oldHealth);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& oldMaxHealth);


	
	// Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turns into -Health
	// Temporary value that only exists on the Server. Not replicated.
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UHealthSet, Damage)
public:
	UHealthSet();
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

};
