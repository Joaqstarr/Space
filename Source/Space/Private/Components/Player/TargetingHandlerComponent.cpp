// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/TargetingHandlerComponent.h"

#include "Actors/Targetable.h"
#include "Camera/CameraComponent.h"
#include "Actors/Targetable.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"

UTargetingHandlerComponent::UTargetingHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

ATargetable* UTargetingHandlerComponent::GetBestTarget()
{
	ATargetable* closest = nullptr;
	float closestDist {10000};

	FVector2D center(0.5f,0.5f);
	//GEngine->AddOnScreenDebugMessage(45, 8.1f, FColor::Magenta, FString::Printf(TEXT("Num: %d"), PotentialTargets.Num()));
	for(ATargetable* target : PotentialTargets)
	{
		FVector2D targPos {target->GetScreenPosition()};
		//GEngine->AddOnScreenDebugMessage(46, 8.1f, FColor::Purple, targPos.ToString());

		if(IsValidScreenPosition(targPos))
		{
	//		GEngine->AddOnScreenDebugMessage(46, 8.1f, FColor::Magenta, TEXT("is valid"));

			float dist = FVector2D::Distance(center, targPos);;
			if(closest == nullptr || dist < closestDist)
			{
				closestDist = dist;
				closest = target;
			}
		}
	}
	return closest;
}

bool UTargetingHandlerComponent::UpdateTarget()
{
	ATargetable* best = GetBestTarget();

	SetCurrentTarget(best);
	return CurrentTarget != nullptr;
}


void UTargetingHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	TargetingCamera = GetOwner()->GetComponentByClass<UCameraComponent>();
}

bool UTargetingHandlerComponent::IsValidScreenPosition(const FVector2D& pos)
{
	return (pos.X > 0 && pos.X < 1 &&
			pos.Y > 0 && pos.Y < 1);
}


// Called every frame
void UTargetingHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindTargetsInRange();
	FilterTargetsWithScreen();
	CheckLockedTargetValidity();
}

void UTargetingHandlerComponent::SetCurrentTarget(ATargetable* newTarget)
{
	if(newTarget != CurrentTarget)
	{
		OnTargetChanged.Broadcast(newTarget);
	}
	CurrentTarget = newTarget;
}

void UTargetingHandlerComponent::FindTargetsInRange()
{
	TArray<FOverlapResult> overlapResults;
	FVector ownerLocation = GetOwner()->GetActorLocation();

	FCollisionShape sphere = FCollisionShape::MakeSphere(TargetingRange);
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());

	//DrawDebugSphere(GetWorld(), ownerLocation, TargetingRange,12, FColor::Cyan);
	if(GetWorld()->OverlapMultiByChannel(overlapResults, ownerLocation, FQuat::Identity, ECC_GameTraceChannel1, sphere, params))
	{
		//GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, FString::Printf(TEXT("result count: %d"), overlapResults.Num()));
		for(ATargetable* target : PotentialTargets)
		{
			if(target && !overlapResults.ContainsByPredicate([&](const FOverlapResult& res)
			{
				return res.GetComponent() == Cast<UPrimitiveComponent>(target);
			}))
			{
				if(target != nullptr)
					target->ExitedRange();
				PotentialTargets.Remove(target);
			}
		}
		
		for(const FOverlapResult& result : overlapResults)
		{
			UPrimitiveComponent* overlappedComponent = 	result.GetComponent();


			if(overlappedComponent)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("PotentialTarget name: %s"), *overlappedComponent->GetName()));

				ATargetable* targetable = Cast<ATargetable>(overlappedComponent->GetOuter());
				if(targetable && targetable->IsTargetable())
				{
					int count = PotentialTargets.Num();
					PotentialTargets.AddUnique(targetable);
					if(PotentialTargets.Num() != count)
					{
						targetable->EnteredRange();
					}
				}
			}
		}
	}
	//GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, FString::Printf(TEXT("PotentialTargets: %d"), PotentialTargets.Num()));
}

void UTargetingHandlerComponent::FilterTargetsWithScreen()
{
	if(!TargetingCamera)return;

	FVector2D viewPortSize;
	GEngine->GameViewport->GetViewportSize(viewPortSize);

	for(ATargetable* target : PotentialTargets)
	{
		if(target == nullptr)continue;
		FVector2D screenPosition;
		
		FVector targetLocation = target->GetActorLocation();
		
		bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(
		GetWorld()->GetFirstPlayerController(),
		targetLocation,
		screenPosition);
		
		target->SetScreenPosition(screenPosition, bIsOnScreen);
	}
	
	PotentialTargets.RemoveAll([&](ATargetable* target)
	{
		FVector2D screenPosition = target->GetScreenPosition();
		return !IsValidScreenPosition(screenPosition);
	});

	for (ATargetable* target : PotentialTargets)
	{
		UE_LOG(LogTemp, Log, TEXT("Target %s is on screen!"), *target->GetOwner()->GetName());
	}
	//GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::Purple, FString::Printf(TEXT("On Screen targets: %d"), PotentialTargets.Num()));

}

void UTargetingHandlerComponent::CheckLockedTargetValidity()
{
	if(!CurrentTarget)return;

	if(!IsValidScreenPosition(CurrentTarget->GetScreenPosition()))SetCurrentTarget(nullptr);
}

