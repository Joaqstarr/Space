// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GASTargetActors/CapsuleTraceTargetActor.h"
#include "GameplayAbilities/Public/Abilities/GameplayAbilityTargetTypes.h"

ACapsuleTraceTargetActor::ACapsuleTraceTargetActor()
{
	FGameplayAbilityTargetingLocationInfo info;
	info.LocationType = EGameplayAbilityTargetingLocationType::ActorTransform;
	info.SourceActor = GetOwner();
	StartLocation = info;

	
}

void ACapsuleTraceTargetActor::StartTargeting(UGameplayAbility* ability)
{
	CollisionShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHeight * 0.5f);
	Super::StartTargeting(ability);
}

FHitResult ACapsuleTraceTargetActor::PerformTrace(AActor* InSourceActor)
{
	bool bTraceComplex = false;

	FCollisionQueryParams params(SCENE_QUERY_STAT(ACapsuleTraceTargetActor), bTraceComplex);
	params.bReturnPhysicalMaterial = true;
	params.AddIgnoredActor(InSourceActor);

	FVector traceStart = InSourceActor->GetActorLocation();
	FVector traceEnd;
	AimWithPlayerController(InSourceActor, params, traceStart, traceEnd);

	FHitResult hitResult;
	//LineTraceWithFilter(hitResult, InSourceActor->GetWorld(), Filter, traceStart, traceEnd, TraceProfile.Name, params);
	SweepWithFilter(hitResult, InSourceActor->GetWorld(), Filter, traceStart, traceEnd, InSourceActor->GetActorQuat(), CollisionShape, TraceProfile.Name, params);
	if(!hitResult.bBlockingHit)
	{
		hitResult.Location = traceEnd;
	}

	
	return hitResult;
}
