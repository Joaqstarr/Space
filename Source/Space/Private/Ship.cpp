// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"

#include "Components/ShipStats.h"
#include "Components/StaticMeshComponent.h"
#include "Components/Physics/GravityComponent.h"
#include "Components/Physics/HoverComponent.h"

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("StaticShipMesh")));
	ShipMesh->SetSimulatePhysics(true);
	ShipMesh->SetEnableGravity(false);

	SetRootComponent(ShipMesh);
	
	GravityComponent = CreateDefaultSubobject<UGravityComponent>("Gravity Component");
	HoverComponent = CreateDefaultSubobject<UHoverComponent>("Hover Component");
	ShipStats = CreateDefaultSubobject<UShipStats>("Ship Statistics");
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ApplyBrakes();
}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AShip::AddRoll(float rollAmount)
{
	AddTorqueControlAroundAxis(rollAmount, FVector(1,0, 0), ShipStats->RollSpeed, ShipStats->TorqueStrength, ShipStats->Damping);

}

void AShip::AddPitch(float pitchAmount)
{
	AddTorqueControlAroundAxis(-pitchAmount, FVector(0,1, 0), ShipStats->PitchSpeed, ShipStats->TorqueStrength, ShipStats->Damping);

}

void AShip::ApplyBrakes()
{
	if(!ShipMesh)return;

	FVector dir {ShipMesh->GetForwardVector()};
	FVector vel {ShipMesh->GetPhysicsLinearVelocity()};

	float dot = FVector::DotProduct(dir, vel);

	if(dot > ShipStats->BrakeStart)return;

	const float dot01 = -dot/2;

	ShipMesh->AddForce(-vel * dot01 * ShipMesh->GetMass());
}

void AShip::AddTorqueControlAroundAxis(float inputAmount, const FVector& axis, float maxSpeed, float strength, float damp) const
{
	if(!ShipMesh)return;
	
	const FVector worldAngularVelocity {ShipMesh->GetPhysicsAngularVelocityInDegrees()};
	const FVector localAngularVelocity {ActorToWorld().InverseTransformVectorNoScale(worldAngularVelocity)};

	
	float currentSpeed = (localAngularVelocity.X * axis.X) + (localAngularVelocity.Y * axis.Y) + (localAngularVelocity.Z * axis.Z);

	float desiredSpeed {inputAmount * maxSpeed};

	float torqueAmt {(desiredSpeed - currentSpeed) * strength - (currentSpeed * damp)};

	const FVector localTorque{axis * torqueAmt};

	const FVector worldTorque {ShipMesh->GetComponentTransform().TransformVectorNoScale(localTorque)};

	ShipMesh->AddTorqueInDegrees(worldTorque, NAME_None, true);
}

void AShip::ApplyMovementForce(const FVector& direction, float inputValue, float maxSpeed, float forceScale) const
{
	if (!ShipMesh || direction.IsNearlyZero())
		return;

	// Get current velocity in the desired direction
	FVector currentVelocity = ShipMesh->GetComponentVelocity();
	float currentSpeedInDirection = FVector::DotProduct(currentVelocity, direction);

	// Calculate the desired speed based on input
	float desiredSpeed = inputValue * maxSpeed;

	// Compute the speed error
	float speedError = desiredSpeed - currentSpeedInDirection;

	// Calculate the force to apply (scaled by ForceScale)
	FVector force = direction.GetSafeNormal() * speedError * forceScale;

	// Apply the force
	ShipMesh->AddForce(force * ShipMesh->GetMass());
}

void AShip::AddYaw(float yawAmount)
{
	AddTorqueControlAroundAxis(yawAmount, FVector(0,0, 1), ShipStats->YawSpeed, ShipStats->TorqueStrength, ShipStats->Damping);
}

void AShip::AddThrust(float forwardThrust, float sidewaysThrust)
{
	FVector forward {ShipMesh->GetForwardVector()};
	FVector right {ShipMesh->GetRightVector()};


	ApplyMovementForce(forward, forwardThrust, ShipStats->ForwardSpeed, ShipStats->ForceScale);
	ApplyMovementForce(right, sidewaysThrust, ShipStats->StrafeSpeed, ShipStats->ForceScale);

	//ShipMesh->AddForce(forward * forwardThrust * ShipStats->ForwardSpeed, NAME_None, true);
	//ShipMesh->AddForce(right * sidewaysThrust * ShipStats->StrafeSpeed, NAME_None, true);

}

