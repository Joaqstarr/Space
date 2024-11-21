// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LookAtComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ULookAtComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULookAtComponent();

	UFUNCTION(BlueprintCallable)
	void UpdateLookPosition(const FVector& newPos);
protected:
	// Called when the game starts
	void SmoothRotateTowardsTarget(const FVector& target, const float DeltaTime);

	UPROPERTY(EditAnywhere, Category="Look At Properties")
	float RotationSpeed = 5.f;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(EditAnywhere)
	FVector TargetPosition;
		
};
