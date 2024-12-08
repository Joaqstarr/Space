// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnMotionWarpingComponent.generated.h"

USTRUCT(Blueprintable)
struct FMotionWarpingParams
{
	GENERATED_USTRUCT_BODY()
public:
	/*Local space offset to place target in relation to source actor*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WarpOffset;
	/*If true will only adjust warp direction but use the original motion's distance.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPreserveDistance = false;
	/*If true will also adjust direction towards target.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bWarpDirection = false;
};

UCLASS()
class UPawnMotionWarpingComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPawnMotionWarpingComponent();
	/**
	 * Warp the pawn to match the target position and orientation.
	 * @param currentRootMotionDelta The current root motion delta transform (from animation).
	 * @param targetTransform The desired warp target transform (world-space).
	 */
	void ApplyMotionWarping(const FTransform& currentRootMotionDelta, const FTransform& targetTransform);
	/**
	 * Set the component or target point to warp to.
	 * @param targetActor The actor to warp towards.
	 */
	void SetWarpTarget(AActor* targetActor);
	UFUNCTION(BlueprintCallable)
	AActor* GetWarpTarget();
	
	void UpdateMotionWarping(float interp);

	void StartWarping(const FMotionWarpingParams& params);
	void StopWarping(){bIsWarping = false;};
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	/** Reference to the target actor for motion warping */
	TWeakObjectPtr<AActor> WarpTargetActor;

	TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
	/** Internal helper function to calculate the warp adjustment */
	FVector  CalculateWarpTransform(const FVector& rootMotionTranslation, const FVector& targetWorldPosition, float interpVal) const;
	FRotator  CalculateWarpDirection(const FRotator rootRotation,const FVector& targetWorldPosition, float interpVal) const;

	bool bIsWarping = false;
	float WarpInterp = 0;

	bool bWarpDirection = false;
	bool bPreserveDistance = false;
	FVector WarpOffset;

	
};
