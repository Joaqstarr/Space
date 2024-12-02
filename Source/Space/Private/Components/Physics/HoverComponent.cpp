// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Physics/HoverComponent.h"
#include "GravityComponent.h"
#include "AbilitySystemComponent.h"
// Sets default values for this component's properties
UHoverComponent::UHoverComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	HoverTag = FGameplayTag::RequestGameplayTag(FName("Physics.Hovering"));
}


// Called when the game starts
void UHoverComponent::BeginPlay()
{
	Super::BeginPlay();

	GravityComponent = GetOwner()->GetComponentByClass<UGravityComponent>();

}


void UHoverComponent::CalculateAndAddHoverForce(const FVector& gravity)
{
	if(!GravityComponent)return;
	
	//if no gravity, no hover
	if(gravity.IsNearlyZero()) return;
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

void UHoverComponent::RemoveHoverTag(const IAbilitySystemInterface& abilityOwner) const
{
	if (abilityOwner.GetAbilitySystemComponent()->HasMatchingGameplayTag(HoverTag))
	{
		abilityOwner.GetAbilitySystemComponent()->RemoveLooseGameplayTag(HoverTag);
	}
}

void UHoverComponent::AddHoverTag(const IAbilitySystemInterface& abilityOwner) const
{
	if (!abilityOwner.GetAbilitySystemComponent()->HasMatchingGameplayTag(HoverTag))
	{
		abilityOwner.GetAbilitySystemComponent()->AddLooseGameplayTag(HoverTag);
	}
}

// Called every frame
void UHoverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector gravity {FVector::Zero()};
	if(GravityComponent != nullptr)
	{
		gravity = GravityComponent->GetGravityDirection();
	}

	if(IAbilitySystemInterface* abilityOwner = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		//if no gravity
		if(gravity.IsNearlyZero()){
			
			RemoveHoverTag(*abilityOwner);
			return;
		}
		//if has gravity add tag
		if(HoverStrength > 0.1f)
		{
			AddHoverTag(*abilityOwner);
		}
	}
	
	CalculateAndAddHoverForce(gravity);
	SmoothOrientToGravity(gravity, DeltaTime);
}

float UHoverComponent::HooksLawDampen(float hitDistance)
{
	float forceAmount = HoverStrength * (HoverHeight - hitDistance) + (Dampening * (lastHitDist - hitDistance));
	forceAmount = FMath::Max(0, forceAmount);
	lastHitDist = hitDistance;

	return forceAmount;
}

void UHoverComponent::SmoothOrientToGravity(const FVector& GravityVector, float DeltaTime) const
{
	if(!bOrientToGravity)return;
	if(GravityVector.IsNearlyZero())return;
	if (!GetOwner()) return;


	// Get the root component and ensure it's a physics body
	UPrimitiveComponent* RootComponent = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	if (!RootComponent || !RootComponent->IsSimulatingPhysics()) return;

	// Normalize the gravity vector
	FVector DownDirection = GravityVector.GetSafeNormal();

	// Calculate the current "down" vector of the actor (its negative up vector)
	FVector ActorDown = -RootComponent->GetUpVector();

	// Calculate the rotation needed to align the actor's "down" with the gravity vector
	FVector TorqueAxis = FVector::CrossProduct(ActorDown, DownDirection);
	float TorqueMagnitude = FVector::DotProduct(ActorDown, DownDirection) - 1.0f; // -1.0f to +1.0f range

	// Scale the torque for smooth alignment
	TorqueMagnitude *= -reorientationStrength; // Adjust multiplier for stronger or weaker torque

	// Apply the torque to the actor's physics body
	FVector Torque = TorqueAxis * TorqueMagnitude;
	RootComponent->AddTorqueInRadians(Torque, NAME_None, true);
}

