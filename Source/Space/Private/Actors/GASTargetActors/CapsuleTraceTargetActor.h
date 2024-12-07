// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "CapsuleTraceTargetActor.generated.h"

class UCapsuleComponent;
/**
 * 
 */
UCLASS()
class ACapsuleTraceTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
public:
	ACapsuleTraceTargetActor();
	virtual void StartTargeting(UGameplayAbility* ability) override;
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> TargetCapsule;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> RootSceneComponent;
protected:
	virtual void ConfirmTargetingAndContinue() override;
	void FindTargetsInCapsule(TArray<FHitResult>& outHits) const;
	FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<AActor*>& targetActors) const;

};
