// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ShipParts/AngleLimiterComponent.h"

// Constructor
UAngleLimiterComponent::UAngleLimiterComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UAngleLimiterComponent::BeginPlay()
{
	Super::BeginPlay();
	TargetComponent = GetChildComponent(0);
	
}

// Called every frame
void UAngleLimiterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TargetComponent)
	{
		ApplyAngleLimits(DeltaTime);
	}
}

#if WITH_EDITOR
void UAngleLimiterComponent::DebugDrawComponent() const
{
	if (!bDrawDebugLines)
		return;
	FlushPersistentDebugLines(GetWorld());
	
	FVector ComponentLocation = GetComponentLocation();
    FQuat ComponentRotation = GetComponentQuat();
	float debugLength = 100;
    // Visualize Pitch Range
    FVector MinPitchDirection = ComponentRotation.RotateVector(FVector( FMath::Cos(FMath::DegreesToRadians(MinPitchLimit)), 0, -FMath::Sin(FMath::DegreesToRadians(MinPitchLimit))));
    FVector MaxPitchDirection = ComponentRotation.RotateVector(FVector( FMath::Cos(FMath::DegreesToRadians(MaxPitchLimit)), 0,-FMath::Sin(FMath::DegreesToRadians(MaxPitchLimit))));

    DrawDebugLine(GetWorld(), ComponentLocation, ComponentLocation + MinPitchDirection * debugLength, FColor::Green, true, -1.0f, 0, 1.0f);
    DrawDebugLine(GetWorld(), ComponentLocation, ComponentLocation + MaxPitchDirection * debugLength, FColor::Green, true, -1.0f, 0, 1.0f);

    // Visualize Yaw Range
    FVector MinYawDirection = ComponentRotation.RotateVector(FVector( FMath::Cos(FMath::DegreesToRadians(MinYawLimit)), FMath::Sin(FMath::DegreesToRadians(MinYawLimit)), 0));
    FVector MaxYawDirection = ComponentRotation.RotateVector(FVector( FMath::Cos(FMath::DegreesToRadians(MaxYawLimit)), FMath::Sin(FMath::DegreesToRadians(MaxYawLimit)),0));

    DrawDebugLine(GetWorld(), ComponentLocation, ComponentLocation + MinYawDirection * debugLength, FColor::Blue, true, -1.0f, 0, 1.0f);
    DrawDebugLine(GetWorld(), ComponentLocation, ComponentLocation + MaxYawDirection * debugLength, FColor::Blue, true, -1.0f, 0, 1.0f);

    // Visualize Roll Range (less common, typically used for tilts)
    FVector MinRollDirection = ComponentRotation.RotateVector(FVector(0, FMath::Sin(FMath::DegreesToRadians(MinRollLimit)),  FMath::Cos(FMath::DegreesToRadians(MinRollLimit))));
    FVector MaxRollDirection = ComponentRotation.RotateVector(FVector(0, FMath::Sin(FMath::DegreesToRadians(MaxRollLimit)),  FMath::Cos(FMath::DegreesToRadians(MaxRollLimit))));

    DrawDebugLine(GetWorld(), ComponentLocation, ComponentLocation + MinRollDirection * debugLength, FColor::Red, true
    	, -1.0f, 0, 1.0f);
    DrawDebugLine(GetWorld(), ComponentLocation, ComponentLocation + MaxRollDirection * debugLength, FColor::Red, true, -1.0f, 0, 1.0f);
}

void UAngleLimiterComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	DebugDrawComponent();
}

void UAngleLimiterComponent::OnRegister()
{
	Super::OnRegister();
	DebugDrawComponent();
}

#endif

void UAngleLimiterComponent::ApplyAngleLimits(float DeltaTime)
{
	// Get the current rotation of the child component
	FRotator CurrentRotation = TargetComponent->GetRelativeRotation();

	// Clamp each axis independently
	FRotator ClampedRotation;
	ClampedRotation.Pitch = FMath::Clamp(CurrentRotation.Pitch, MinPitchLimit, MaxPitchLimit);
	ClampedRotation.Yaw = FMath::Clamp(CurrentRotation.Yaw, MinYawLimit, MaxYawLimit);
	ClampedRotation.Roll = FMath::Clamp(CurrentRotation.Roll, MinRollLimit, MaxRollLimit);

	// Apply the clamped rotation back to the child component
	TargetComponent->SetRelativeRotation(ClampedRotation);
}
