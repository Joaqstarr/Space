// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ShipParts/LookAtComponent.h"

// Sets default values for this component's properties
ULookAtComponent::ULookAtComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);
	
}

void ULookAtComponent::UpdateLookPosition(const FVector& newTargetPos, const FVector& targVelocity, float bulletSpeed)
{
	TargetPosition = LookAheadByVelocity(newTargetPos, targVelocity, GetComponentLocation(), bulletSpeed);
}

FVector ULookAtComponent::LookAheadByVelocity(const FVector& newTargetPos, const FVector& targVelocity, const FVector& currentLocation, float speed)
{
	FVector toTarget = newTargetPos - currentLocation;

	float a = targVelocity.Dot(targVelocity) - speed * speed;
	float b = 2 * toTarget.Dot(targVelocity);
	float c = toTarget.Dot(toTarget);

	float discriminant = b*b - 4 * a * c;

	if(discriminant < 0)
	{
		return newTargetPos;
	}

	float discriminantSqrt {FMath::Sqrt(discriminant)};

	float t1 = (-b + discriminantSqrt) / (2*a);
	float t2 = (-b - discriminantSqrt) / (2 * a);

	float t = (t1 > 0) ? t1 : t2;

	if(t < 0)
	{
		return newTargetPos;
	}
	
	return newTargetPos + targVelocity * t;
}


void ULookAtComponent::SmoothRotateTowardsTarget(const FVector& target, const float deltaTime)
{
	FRotator currentRotation = GetComponentRotation();

	const FVector direction = (target - GetComponentLocation()).GetSafeNormal();

	FRotator targetRotation = direction.Rotation();

	FQuat newRotation {FQuat::Slerp(currentRotation.Quaternion(), targetRotation.Quaternion(), RotationSpeed * deltaTime)};

	SetWorldRotation(newRotation);
}

// Called every frame
void ULookAtComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SmoothRotateTowardsTarget(TargetPosition, DeltaTime);
}

