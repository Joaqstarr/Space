	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingHandlerComponent.generated.h"


	class UCameraComponent;
	class UTargetableComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTargetingHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetingHandlerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UTargetableComponent>> PotentialTargets;
private:
	UPROPERTY(EditAnywhere)
	float TargetingRange = 10000;
	


	void FindTargetsInRange();
	void FilterTargetsWithScreen();

	TObjectPtr<UCameraComponent> TargetingCamera;
};
