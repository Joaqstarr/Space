// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "SpaceAttributeSet.h"
#include "DashSet.generated.h"
/**
 * 
 */
UCLASS()
class UDashSet : public USpaceAttributeSet
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Dash",  ReplicatedUsing= OnRep_DashStrength)
	FGameplayAttributeData DashStrength;
	ATTRIBUTE_ACCESSORS(UDashSet, DashStrength)

	UFUNCTION()
	void OnRep_DashStrength(const FGameplayAttributeData& oldDashStrength);
public:
	UDashSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
