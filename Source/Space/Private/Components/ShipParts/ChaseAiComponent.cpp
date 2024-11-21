// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ShipParts/ChaseAiComponent.h"

#include "Ship.h"

// Sets default values for this component's properties
UChaseAiComponent::UChaseAiComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UChaseAiComponent::UpdateTargetPosition(const FVector& targ)
{
	TargetPosition = targ;
}


// Called when the game starts
void UChaseAiComponent::BeginPlay()
{
	Super::BeginPlay();

	Ship = Cast<AShip>(GetOwner());	
}


void UChaseAiComponent::FlyTowardsTarget(const FVector& target) const
{
	if(!Ship)return;
	
	FVector forward {GetOwner()->GetActorForwardVector()};
	FVector right {GetOwner()->GetActorRightVector()};
	FVector up {GetOwner()->GetActorUpVector()};
	FVector dir {(GetOwner()->GetActorLocation() - target).GetSafeNormal()};

	float forwardDot = FVector::DotProduct(forward, dir);
	float rightDot = FVector::DotProduct(right, dir);
	float upDot = FVector::DotProduct(up, dir);

	Ship->AddThrust(-forwardDot, 0, 0);
	
	Ship->AddYaw(rightDot);
	Ship->AddPitch(upDot);
	Ship->AddRoll(0);
}

// Called every frame
void UChaseAiComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	FlyTowardsTarget(TargetPosition);
}



