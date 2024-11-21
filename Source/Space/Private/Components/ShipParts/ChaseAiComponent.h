// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChaseAiComponent.generated.h"


class AShip;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UChaseAiComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChaseAiComponent();
	UFUNCTION(BlueprintCallable)
	void UpdateTargetPosition(const FVector& targ);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void FlyTowardsTarget(const FVector& target) const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FVector TargetPosition;
	TObjectPtr<AShip> Ship;
};
