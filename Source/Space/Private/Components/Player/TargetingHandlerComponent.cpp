// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/TargetingHandlerComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/TargetableComponent.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"

UTargetingHandlerComponent::UTargetingHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UTargetingHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	TargetingCamera = GetOwner()->GetComponentByClass<UCameraComponent>();
}


// Called every frame
void UTargetingHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindTargetsInRange();
	FilterTargetsWithScreen();
	
}

void UTargetingHandlerComponent::FindTargetsInRange()
{
	PotentialTargets.Empty();
	
	TArray<FOverlapResult> overlapResults;
	FVector ownerLocation = GetOwner()->GetActorLocation();

	FCollisionShape sphere = FCollisionShape::MakeSphere(TargetingRange);
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());

	DrawDebugSphere(GetWorld(), ownerLocation, TargetingRange,12, FColor::Cyan);
	if(GetWorld()->OverlapMultiByChannel(overlapResults, ownerLocation, FQuat::Identity, ECC_GameTraceChannel1, sphere, params))
	{
		GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, FString::Printf(TEXT("result count: %d"), overlapResults.Num()));

		for(const FOverlapResult& result : overlapResults)
		{
			UPrimitiveComponent* overlappedComponent = 	result.GetComponent();


			if(overlappedComponent)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("PotentialTarget name: %s"), *overlappedComponent->GetName()));

				UTargetableComponent* targetable = Cast<UTargetableComponent>(overlappedComponent->GetOuter());
				if(targetable && targetable->IsTargetable())
				{
					PotentialTargets.Add(targetable);
				}
			}
		}
	}
	GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red, FString::Printf(TEXT("PotentialTargets: %d"), PotentialTargets.Num()));
}

void UTargetingHandlerComponent::FilterTargetsWithScreen()
{
	if(!TargetingCamera)return;

	FVector2D viewPortSize;
	GEngine->GameViewport->GetViewportSize(viewPortSize);

	for(UTargetableComponent* target : PotentialTargets)
	{
		FVector2D screenPosition;
		
		FVector targetLocation = target->GetComponentLocation();
		
		bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(
		GetWorld()->GetFirstPlayerController(),
		targetLocation,
		screenPosition);
		
		target->SetScreenPosition(screenPosition, bIsOnScreen);
	}
	
	PotentialTargets.RemoveAll([&](UTargetableComponent* target)
	{
		FVector2D screenPosition = target->GetScreenPosition();
		return (screenPosition.X < 0 || screenPosition.X > 1 ||
			screenPosition.Y < 0 || screenPosition.Y > 1);
	});

	for (UTargetableComponent* target : PotentialTargets)
	{
		UE_LOG(LogTemp, Log, TEXT("Target %s is on screen!"), *target->GetOwner()->GetName());
	}
	GEngine->AddOnScreenDebugMessage(3, 0.f, FColor::Purple, FString::Printf(TEXT("On Screen targets: %d"), PotentialTargets.Num()));

}

