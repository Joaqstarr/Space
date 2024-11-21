// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ShipParts/LookAtComponent.h"

// Sets default values for this component's properties
ULookAtComponent::ULookAtComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}

void ULookAtComponent::UpdateLookPosition(const FVector& newPos)
{
	TargetPosition = newPos;
}


void ULookAtComponent::SmoothRotateTowardsTarget(const FVector& target, const float deltaTime)
{
	FRotator currentRotation = GetRelativeRotation();

	const FVector direction = (target - GetComponentLocation()).GetSafeNormal();
	FRotator targetRotation = direction.Rotation();
	

	FQuat newRotation {FQuat::Slerp(currentRotation.Quaternion(), targetRotation.Quaternion(), RotationSpeed * deltaTime)};

	SetRelativeRotation(newRotation);
}

// Called every frame
void ULookAtComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SmoothRotateTowardsTarget(TargetPosition, DeltaTime);
}

