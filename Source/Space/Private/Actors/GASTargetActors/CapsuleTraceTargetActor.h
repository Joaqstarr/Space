// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_Trace.h"
#include "CapsuleTraceTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class ACapsuleTraceTargetActor : public AGameplayAbilityTargetActor_Trace
{
	GENERATED_BODY()
public:
	ACapsuleTraceTargetActor();
	virtual void StartTargeting(UGameplayAbility* ability) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CapsuleRadius = 50;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CapsuleHeight= 100;

	virtual FHitResult PerformTrace(AActor* InSourceActor) override;
private:
	FCollisionShape CollisionShape;
};
