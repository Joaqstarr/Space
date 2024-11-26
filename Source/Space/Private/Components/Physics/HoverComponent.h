// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HoverComponent.generated.h"


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
	float reorientationStrength {200};
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(EditAnywhere)
	float HoverHeight {500};
	UPROPERTY(EditAnywhere)
	float HoverStrength {2};
	UPROPERTY(EditAnywhere)
	float Dampening {0.5f};
	
	UPROPERTY()
	TObjectPtr<UGravityComponent> GravityComponent;
	
	float lastHitDist;
	float HooksLawDampen(float hitDistance);

	void SmoothOrientToGravity(const FVector& GravityVector, float DeltaTime) const;
	void CalculateAndAddHoverForce(const FVector& GravityVector);

};
