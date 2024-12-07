// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GASTargetActors/CapsuleTraceTargetActor.h"

#include "Components/CapsuleComponent.h"
#include "GameplayAbilities/Public/Abilities/GameplayAbilityTargetTypes.h"

ACapsuleTraceTargetActor::ACapsuleTraceTargetActor()
{
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootSceneComponent);
	
	TargetCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TargetCapsule"));
	TargetCapsule->SetupAttachment(GetRootComponent());
	TargetCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TargetCapsule->SetCollisionObjectType(ECC_WorldDynamic);
	TargetCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	//TargetCapsule->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	TargetCapsule->InitCapsuleSize(100.0f, 200.0f);
}

void ACapsuleTraceTargetActor::StartTargeting(UGameplayAbility* ability)
{
	Super::StartTargeting(ability);
	
	if (SourceActor)
	{
		FVector forwardVector = SourceActor->GetActorForwardVector();
		FVector startLocation = SourceActor->GetActorLocation() + forwardVector * 300.0f; // Adjust distance as needed
		TargetCapsule->SetWorldLocation(startLocation);
		TargetCapsule->SetWorldRotation(forwardVector.Rotation());
	}
}

void ACapsuleTraceTargetActor::ConfirmTargetingAndContinue()
{
	TArray<FHitResult> hitResults;
	FindTargetsInCapsule(hitResults);

	TArray<AActor*> targetActors;
	for (const FHitResult& hit : hitResults)
	{
		AActor* hitActor = hit.GetActor();
		if (hitActor && !targetActors.Contains(hitActor))
		{
			targetActors.Add(hitActor);
		}
	}

	// Create the target data
	if (targetActors.Num() > 0)
	{
		FGameplayAbilityTargetDataHandle targetDataHandle = MakeTargetData(targetActors);
		TargetDataReadyDelegate.Broadcast(targetDataHandle);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}

	Destroy();
}

void ACapsuleTraceTargetActor::FindTargetsInCapsule(TArray<FHitResult>& outHits) const
{
	FVector capsuleLocation = TargetCapsule->GetComponentLocation();
	FQuat capsuleRotation = TargetCapsule->GetComponentQuat();
	float capsuleRadius = TargetCapsule->GetScaledCapsuleRadius();
	float capsuleHalfHeight = TargetCapsule->GetScaledCapsuleHalfHeight();

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(Targeting), false, SourceActor);
	GetWorld()->SweepMultiByChannel(
		outHits,
		capsuleLocation,
		capsuleLocation,
		capsuleRotation,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeCapsule(capsuleRadius, capsuleHalfHeight),
		QueryParams
	);

	if(bDebug)
	{
		GEngine->AddOnScreenDebugMessage(5, 4.f, FColor::Emerald, FString::Printf(TEXT("Num hits: %d"), outHits.Num()));
		// Optionally draw debug
		DrawDebugCapsule(
			 GetWorld(),
			capsuleLocation,
			capsuleHalfHeight,
			capsuleRadius,
			capsuleRotation,
			FColor::Blue,
			false,
			2.0f
		);
	}
}

FGameplayAbilityTargetDataHandle ACapsuleTraceTargetActor::MakeTargetData(const TArray<AActor*>& targetActors) const
{
	FGameplayAbilityTargetDataHandle targetDataHandle;

	for (AActor* targetActor : targetActors)
	{
		if (targetActor)
		{
			TSharedPtr<FGameplayAbilityTargetData_ActorArray> targetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
			targetData->TargetActorArray.Add(targetActor);
			targetDataHandle.Data.Add(targetData);
		}
	}

	return targetDataHandle;
}
