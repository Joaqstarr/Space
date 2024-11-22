// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ShipParts/ChaseAiComponent.h"

#include "CollisionDebugDrawingPublic.h"
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


void UChaseAiComponent::ApplyLinearThrust(FVector dir) const
{
	FVector forward = GetOwner()->GetActorForwardVector();
	FVector up = GetOwner()->GetActorUpVector();
	FVector right = GetOwner()->GetActorRightVector();

	float forwardDot = FVector::DotProduct(forward, dir);
	float verticalInput = 0;
	float horizontalInput = 0;
	// Clamp thrust if an obstacle is in front
	FHitResult forwardRes = LineTraceDir(forward, ObstacleDetectionRange);
	
	FHitResult upRes = LineTraceDir(up, GroundAvoidanceDist);
	FHitResult downRes = LineTraceDir(-up, GroundAvoidanceDist);
	FHitResult rightRes = LineTraceDir(right, GroundAvoidanceDist);
	FHitResult leftRes = LineTraceDir(-right, GroundAvoidanceDist);

	if (forwardRes.bBlockingHit)
	{
		forwardDot = FMath::Clamp(forwardDot, 0.0f, 1.0f); // Prevent moving into the obstacle
	}

	if(upRes.bBlockingHit)
	{
		verticalInput += -1;
	}
	if(downRes.bBlockingHit)
	{
		verticalInput += 1;
	}
	if(rightRes.bBlockingHit)
	{
		horizontalInput += -1;
	}
	if(leftRes.bBlockingHit)
	{
		horizontalInput += 1;
	}
	Ship->AddThrust(forwardDot, horizontalInput, verticalInput);
}

FVector UChaseAiComponent::CalculateAvoidance(const FVector& forward, const FVector& sideDir, const FHitResult& forwardRes) const
{
	FVector avoidanceDir = FVector::ZeroVector;

	if (forwardRes.bBlockingHit)
	{
		// Directions at ±45 degrees
		FVector right45Deg = (forward + sideDir).GetSafeNormal();
		FVector left45Deg = (forward - sideDir).GetSafeNormal();

		FHitResult leftRes = LineTraceDir(left45Deg, ObstacleDetectionRange);
		FHitResult rightRes = LineTraceDir(right45Deg, ObstacleDetectionRange);

		// Prioritize avoiding based on which direction is clearer
		if (!leftRes.bBlockingHit)
		{
			avoidanceDir = left45Deg;
		}
		else if (!rightRes.bBlockingHit)
		{
			avoidanceDir = right45Deg;
		}
		else
		{
			// Both sides blocked, fallback to minimal adjustment
			avoidanceDir = -(forwardRes.ImpactPoint - GetOwner()->GetActorLocation()).GetSafeNormal();
		}
	}

	return avoidanceDir;
}

FVector UChaseAiComponent::ApplyPitchYawRollThrust(const FVector& dir) const
{
	FVector forward = GetOwner()->GetActorForwardVector();
	FVector right = GetOwner()->GetActorRightVector();
	FVector up = GetOwner()->GetActorUpVector();

	FHitResult forwardRes = LineTraceDir(forward, ObstacleDetectionRange);

	// Calculate avoidance contributions
	FVector sidewaysAvoidance = CalculateAvoidance(forward, right, forwardRes);
	FVector verticalAvoidance = CalculateAvoidance(forward, up, forwardRes);

	// Blend the avoidance directions
	FVector avoidanceDir = (sidewaysAvoidance + verticalAvoidance).GetSafeNormal();

	// Combine target direction with avoidance
	FVector finalDir = (dir + avoidanceDir * 1).GetSafeNormal();

	// Adjust ship's rotation
	float yaw = FVector::DotProduct(right, finalDir);
	float pitch = FVector::DotProduct(up, finalDir);

	Ship->AddYaw(yaw);
	Ship->AddPitch(pitch);
	Ship->AddRoll(0);

	return finalDir;
}

void UChaseAiComponent::FlyTowardsTarget(const FVector& target) const
{
	if (!Ship) return;

	// Desired direction to the target
	FVector targetDir = (target - GetOwner()->GetActorLocation()).GetSafeNormal();
	DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + (targetDir * 1000), FColor::Cyan);
	// Apply avoidance to adjust target direction
	FVector adjustedDir = ApplyPitchYawRollThrust(targetDir);

	// Apply thrust based on the adjusted direction
	ApplyLinearThrust(adjustedDir);
}


// Called every frame
void UChaseAiComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	FlyTowardsTarget(TargetPosition);
}

FHitResult UChaseAiComponent::LineTraceDir(const FVector& dir, float distance) const
{
	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, GetOwner());
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;
	
	FHitResult RV_Hit(ForceInit);

	FVector location = GetOwner()->GetActorLocation();
	TArray<FHitResult> hits{TArray<FHitResult>()};
	DrawLineTraces(GetWorld(), location, location + (dir * distance), hits, 0);
	GetWorld()->LineTraceSingleByChannel(RV_Hit, location, location + (dir * distance), ECC_Visibility, RV_TraceParams);


	return RV_Hit;
}



