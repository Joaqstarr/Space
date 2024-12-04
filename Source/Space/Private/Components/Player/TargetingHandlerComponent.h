	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetingHandlerComponent.generated.h"


class UCameraComponent;
class ATargetable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetChangedSignature, ATargetable*, newTarget);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTargetingHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetingHandlerComponent();
	

	UFUNCTION(BlueprintCallable)
	bool UpdateTarget();

	UFUNCTION(BlueprintCallable)
	ATargetable* GetCurrentTarget(){return CurrentTarget;};

	UPROPERTY(BlueprintAssignable,Category=Targeting)
	FOnTargetChangedSignature OnTargetChanged;

protected:
	ATargetable* CalculateBestTarget();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ATargetable> CurrentTarget = nullptr;
	// Called when the game starts
	virtual void BeginPlay() override;

	bool IsValidScreenPosition(const FVector2D& pos);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<ATargetable>> PotentialTargets;
private:
	UPROPERTY(EditAnywhere)
	float TargetingRange = 10000;
	

	void SetCurrentTarget(ATargetable* newTarget);
	void FindTargetsInRange();
	void FilterTargetsWithScreen();
	void CheckLockedTargetValidity();
	
	TObjectPtr<UCameraComponent> TargetingCamera;
};
