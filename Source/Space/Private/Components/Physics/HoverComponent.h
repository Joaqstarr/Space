// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "HoverComponent.generated.h"


class IAbilitySystemInterface;
class UGravityComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHoverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHoverComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category=Reorienatation)
	bool bOrientToGravity = true;
	UPROPERTY(EditAnywhere, Category=Reorienatation)
	float reorientationStrength {30};
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	void SmoothOrientToGravity(const FVector& GravityVector, float DeltaTime) const;
	void CalculateAndAddHoverForce(const FVector& GravityVector);
	void RemoveHoverTag(const IAbilitySystemInterface& abilityOwner) const;
	void AddHoverTag(const IAbilitySystemInterface& abilityOwner) const;
private:
	UPROPERTY(EditAnywhere)
	float HoverHeight {600};
	UPROPERTY(EditAnywhere)
	float HoverStrength {4};
	UPROPERTY(EditAnywhere)
	float Dampening {0.8f};
	
	UPROPERTY()
	TObjectPtr<UGravityComponent> GravityComponent;
	
	float lastHitDist = 0;
	float HooksLawDampen(float hitDistance);

	FGameplayTag HoverTag;
};
