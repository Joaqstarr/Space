// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/PawnMotionWarpingComponent.h"

UPawnMotionWarpingComponent::UPawnMotionWarpingComponent()
{
	PrimaryComponentTick.bCanEverTick = true; // Only activate when needed
}

void UPawnMotionWarpingComponent::ApplyMotionWarping(const FTransform& currentRootMotionDelta,
	const FTransform& targetTransform)
{

	if (!GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("CustomMotionWarpingComponent: Owner is null."));
		return;
	}

	// Calculate the warp adjustment transform
	//FTransform WarpTransform = CalculateWarpTransform(currentRootMotionDelta, targetTransform, WarpInterp);

	// Apply warp adjustment to the owner's physics component
	if (UPrimitiveComponent* PhysicsComponent = GetOwner()->FindComponentByClass<UPrimitiveComponent>())
	{
		//ApplyWarpToPhysicsComponent(PhysicsComponent, WarpTransform);
	}
	else
	{
		// Fall back to applying directly to the actor's transform if no physics component is found
		//GetOwner()->AddActorWorldTransform(WarpTransform, true, nullptr, ETeleportType::TeleportPhysics);
	}
}

void UPawnMotionWarpingComponent::SetWarpTarget(AActor* targetActor)
{
	WarpTargetActor = targetActor;

}

AActor* UPawnMotionWarpingComponent::GetWarpTarget()
{
	if(WarpTargetActor == nullptr)
	{
		return nullptr;
	}
	
	return &(*WarpTargetActor);
}

void UPawnMotionWarpingComponent::UpdateMotionWarping(float interp)
{
	if (!GetWorld() || !GetWorld()->IsGameWorld()) // Skip in editor or preview worlds
	{
		return;
	}

	if (!GetOwner() || !GetOwner()->IsValidLowLevel()) // Ensure the owner is valid
	{
		return;
	}

	if(!WarpTargetActor.IsValid() || WarpTargetActor == nullptr) return;

	WarpInterp = interp;
	//GEngine->AddOnScreenDebugMessage(63, 2, FColor::Red, FString::Printf(TEXT("Warping Interpolation: %f"), WarpInterp));
}

void UPawnMotionWarpingComponent::StartWarping(const FMotionWarpingParams& params)
{
	bIsWarping = true;
	bPreserveDistance = params.bPreserveDistance;
	bWarpDirection = params.bWarpDirection;
	WarpOffset = params.WarpOffset;
}

void UPawnMotionWarpingComponent::BeginPlay()
{
	Super::BeginPlay();

	SkeletalMeshComponent = Cast<USkeletalMeshComponent>( GetOwner()->GetRootComponent());
}

void UPawnMotionWarpingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	if (SkeletalMeshComponent != nullptr && SkeletalMeshComponent->GetAnimInstance())
	{
		const FRootMotionMovementParams rootMotion = SkeletalMeshComponent->ConsumeRootMotion();
		
		if (rootMotion.GetRootMotionTransform().IsValid())
		{
			FVector rootMotionTranslation = rootMotion.GetRootMotionTransform().GetTranslation();
			FRotator rootMotionRotation = rootMotion.GetRootMotionTransform().GetRotation().Rotator();
			
			if(rootMotionTranslation.IsNearlyZero())return;

			FVector worldAlignedTranslation = GetOwner()->GetActorRotation().RotateVector(rootMotionTranslation);
			
			if(bIsWarping && WarpTargetActor != nullptr)
			{
				worldAlignedTranslation = CalculateWarpTransform(rootMotionTranslation, WarpTargetActor->GetActorLocation(), WarpInterp);
				FRotator calculatedWarpRotation = CalculateWarpDirection(rootMotionRotation, WarpTargetActor->GetActorLocation(),WarpInterp);


				GetOwner()->SetActorRotation(calculatedWarpRotation, ETeleportType::TeleportPhysics);
			}else
			{
				GetOwner()->AddActorWorldRotation(rootMotionRotation, true, nullptr,ETeleportType::TeleportPhysics);

			}
			GetOwner()->AddActorWorldOffset(worldAlignedTranslation, true, nullptr,ETeleportType::TeleportPhysics);
		}
	}
}


FVector  UPawnMotionWarpingComponent::CalculateWarpTransform(const FVector& rootMotionTranslation,
	const FVector& targetWorldPosition, float interpVal) const
{
	interpVal = FMath::Clamp(interpVal, 0.0f, 1.0f);

	
	FVector currentWorldPosition = GetOwner()->GetActorLocation() + GetOwner()->GetActorRotation().RotateVector(WarpOffset);
	FVector currentDirection = GetOwner()->GetActorRotation().Vector();
	float distance = FVector::Dist(currentWorldPosition, targetWorldPosition);

	// Calculate the direction to the target from the current position
	FVector directionToTarget = (targetWorldPosition - currentWorldPosition).GetSafeNormal();

	// Scale the adjusted direction by the original translation magnitude or the distance lerped by current interpolation
	float distanceScale = (bPreserveDistance)? rootMotionTranslation.Size() : distance * interpVal;
	FVector adjustedTranslation = directionToTarget * distanceScale;

	return adjustedTranslation;
}

FRotator UPawnMotionWarpingComponent::CalculateWarpDirection(const FRotator rootRotation,const FVector& targetWorldPosition, float interpVal) const
{
	FRotator currentRotation = GetOwner()->GetActorRotation();

	if(!bWarpDirection)return rootRotation + currentRotation;
	
	FVector currentWorldPosition = GetOwner()->GetActorLocation();
	FVector directionToTarget = (targetWorldPosition - currentWorldPosition).GetSafeNormal();
	FVector startDir = currentRotation.Vector();
	

	// Use FVector::Slerp for smooth direction adjustment
	FVector transformedDir = FVector::SlerpVectorToDirection(startDir, directionToTarget, interpVal);
	
	
	return transformedDir.Rotation();
}


