// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"

#include "AttributeSets/DashSet.h"
#include "Player/Abilities/ShipAbilitySystemComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ShipStats.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/Physics/GravityComponent.h"
#include "Components/Physics/HoverComponent.h"
#include "Net/UnrealNetwork.h"
#include "Player/Abilities/DashAbility.h"
#include "Utility/VectorPayload.h"

// Sets default values
AShip::AShip() : Super()
{
	NetUpdateFrequency = 60;
	MinNetUpdateFrequency = 30;
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	
	ShipMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName(TEXT("ShipMesh")));
	ShipMeshComponent->SetSimulatePhysics(true);
	ShipMeshComponent->SetEnableGravity(false);
	ShipMeshComponent->SetCollisionObjectType(ECC_Pawn);
	ShipMeshComponent->SetSimulatePhysics(true);
	ShipMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	ShipMeshComponent->SetGenerateOverlapEvents(true);
	ShipMeshComponent->SetIsReplicated(true);
	
	SetRootComponent(ShipMeshComponent);
	
	
	GravityComponent = CreateDefaultSubobject<UGravityComponent>("Gravity Component");
	HoverComponent = CreateDefaultSubobject<UHoverComponent>("Hover Component");
	ShipStats = CreateDefaultSubobject<UShipStats>("Ship Statistics");

	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
	DashAttributeSet = CreateDefaultSubobject<UDashSet>(TEXT("DashSet"));

	if (AbilitySystemComponent && DashAttributeSet)
	{
		AbilitySystemComponent->AddAttributeSetSubobject(DashAttributeSet);
	}
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



void AShip::AddYaw(float yawAmount)
{
	AddTorqueControlAroundAxis(yawAmount, FVector(0,0, 1), ShipStats->YawSpeed, ShipStats->TorqueStrength, ShipStats->Damping);

}


//returns speed lost
float AShip::ApplyBrakes(FVector movementDir) const
{
	if(!ShipMeshComponent)return 0;
	
	movementDir.Normalize();
	FVector vel {ShipMeshComponent->GetComponentVelocity()};
	
	FVector velDir {vel};
	velDir.Normalize();
	
	const float dot = FVector::DotProduct(velDir, movementDir);
	const float dotToInterp = 1 - FMath::Clamp((dot-0.5f) * 2, 0, 1);

	FVector transformedVel {vel * dotToInterp};
	//GEngine->AddOnScreenDebugMessage(2, 0.1f, FColor::Cyan, FString::Printf(TEXT("dot amount: %f"), dot));
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + movementDir * 500, FColor::Cyan);
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + velDir * 200, FColor::Red);

	ShipMeshComponent->AddForce(-transformedVel * ShipMeshComponent->GetMass());

	return transformedVel.Length();
}

void AShip::AddTorqueControlAroundAxis(float inputAmount, const FVector& axis, float maxSpeed, float strength, float damp) const
{
	if(!ShipMeshComponent)return;
	
	const FVector worldAngularVelocity {ShipMeshComponent->GetPhysicsAngularVelocityInDegrees()};
	const FVector localAngularVelocity {ActorToWorld().InverseTransformVectorNoScale(worldAngularVelocity)};

	
	float currentSpeed = (localAngularVelocity.X * axis.X) + (localAngularVelocity.Y * axis.Y) + (localAngularVelocity.Z * axis.Z);

	float desiredSpeed {inputAmount * maxSpeed};

	float torqueAmt {(desiredSpeed - currentSpeed) * strength - (currentSpeed * damp)};

	const FVector localTorque{axis * torqueAmt};

	const FVector worldTorque {ShipMeshComponent->GetComponentTransform().TransformVectorNoScale(localTorque)};

	ShipMeshComponent->AddTorqueInDegrees(worldTorque, NAME_None, true);
}

void AShip::ApplyMovementForce(const FVector& direction, float inputValue, float maxSpeed, float forceScale) const
{
	if (!ShipMeshComponent || direction.IsNearlyZero())
		return;

	// Get current velocity in the desired direction
	FVector currentVelocity = ShipMeshComponent->GetComponentVelocity();
	float currentSpeedInDirection = FVector::DotProduct(currentVelocity, direction);

	// Calculate the desired speed based on input
	float desiredSpeed = inputValue * maxSpeed;

	// Compute the speed error
	float speedError = desiredSpeed - currentSpeedInDirection;

	// Calculate the force to apply (scaled by ForceScale)
	FVector force = direction.GetSafeNormal() * speedError * forceScale;

	// Apply the force
	ShipMeshComponent->AddForce(force * ShipMeshComponent->GetMass());
}



void AShip::AddThrust(float forwardThrust, float sidewaysThrust, float verticalThrust)
{

	
	FVector forward {ShipMeshComponent->GetForwardVector()};
	FVector right {ShipMeshComponent->GetRightVector()};
	FVector up {ShipMeshComponent->GetUpVector()};

	ApplyMovementForce(forward, forwardThrust, ShipStats->ForwardSpeed, ShipStats->ForceScale);
	ApplyMovementForce(right, sidewaysThrust, ShipStats->StrafeSpeed, ShipStats->ForceScale);
	ApplyMovementForce(up, verticalThrust, ShipStats->VerticalSpeed, ShipStats->ForceScale);

	FVector movementDir(forwardThrust, sidewaysThrust, verticalThrust);
	movementDir.Normalize();

	
	//if(movementDir.IsNearlyZero())return;
	
	float lost = ApplyBrakes(ActorToWorld().TransformVectorNoScale(movementDir));

	//GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Cyan, FString::Printf(TEXT("brake amount: %f"), lost));

	ShipMeshComponent->AddForce(lost * movementDir * ShipMeshComponent->GetMass());
	//ShipMesh->AddForce(forward * forwardThrust * ShipStats->ForwardSpeed, NAME_None, true);
	//ShipMesh->AddForce(right * sidewaysThrust * ShipStats->StrafeSpeed, NAME_None, true);

}



void AShip::TryDash(FVector inputDirection)
{
	FGameplayEventData eventData;
	eventData.Instigator = this;
	eventData.Target = this;
	eventData.EventTag = FGameplayTag::RequestGameplayTag(FName("Ship.Action.Dash"));

	//required to use new here, GAS cleans automatically
	FGameplayAbilityTargetData_SingleTargetHit* targetData = new FGameplayAbilityTargetData_SingleTargetHit();
	targetData->HitResult.ImpactPoint = inputDirection;  // Use ImpactPoint to store the FVector data

	// Wrap TargetData into a TargetDataHandle
	eventData.TargetData.Add(targetData);

	UAbilitySystemComponent* asc = GetAbilitySystemComponent();

	if (asc)
	{
		int num = asc->HandleGameplayEvent(eventData.EventTag, &eventData);
		TArray<FGameplayAbilitySpecHandle> outAbilityHandles;
		AbilitySystemComponent->GetAllAbilities(outAbilityHandles);

	}else
	{
		UE_LOG(LogTemp, Error, TEXT("Ship: Unable to find ASC when activating dash."));
	}
}




