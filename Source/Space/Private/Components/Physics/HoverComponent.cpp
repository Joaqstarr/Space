// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Physics/HoverComponent.h"
#include "GravityComponent.h"

// Sets default values for this component's properties
UHoverComponent::UHoverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHoverComponent::BeginPlay()
{
	Super::BeginPlay();

	GravityComponent = GetOwner()->GetComponentByClass<UGravityComponent>();

}


void UHoverComponent::CalculateAndAddHoverForce()
{
	if(!GravityComponent)return;
	
	FVector gravity = GravityComponent->GetGravityDirection();
	
	//if no gravity, no hover
	if(gravity.Length() == 0) return;
	FVector downDir = gravity;

	downDir.Normalize();

	//setup line trace
	FCollisionQueryParams traceParams {FCollisionQueryParams(FName(TEXT("RB_Trace")), true, GetOwner())};
	traceParams.bTraceComplex = true;
	traceParams.bReturnPhysicalMaterial = false;

	const FVector startLocation {GetOwner()->GetActorLocation()};
	const FVector endLocation {GetOwner()->GetActorLocation() + (downDir * HoverHeight)};
	FHitResult lineHit(ForceInit);
	
	//DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Green, false);
	if(GetWorld()->LineTraceSingleByChannel(lineHit, startLocation,  endLocation, ECC_WorldStatic, traceParams))
	{
		if(UMeshComponent* ownerRoot = Cast<UMeshComponent>(GetOwner()->GetRootComponent()))
		{
			const float dist = lineHit.Distance;
			const FVector force {-downDir * HooksLawDampen(dist)};
			ownerRoot->AddForce(force, FName("None"), true);
		}
		
		
	}
}

// Called every frame
void UHoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CalculateAndAddHoverForce();
	
}

float UHoverComponent::HooksLawDampen(float hitDistance)
{
	float forceAmount = HoverStrength * (HoverHeight - hitDistance) + (Dampening * (lastHitDist - hitDistance));
	forceAmount = FMath::Max(0, forceAmount);
	lastHitDist = hitDistance;

	return forceAmount;
}

