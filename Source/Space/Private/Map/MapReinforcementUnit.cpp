// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MapReinforcementUnit.h"

#include "MapEnemyBase.h"
#include "MapTransformComponent.h"
#include "OverworldMap.h"
#include "Components/TokenConsumer.h"

// Sets default values
AMapReinforcementUnit::AMapReinforcementUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	MapTransformComponent = CreateDefaultSubobject<UMapTransformComponent>(FName("MapTransformComponent"));
}

void AMapReinforcementUnit::OnTransferComplete_Implementation()
{
	GetWorld()->DestroyActor(this);
}

// Called when the game starts or when spawned
void AMapReinforcementUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector MoveTowards(const FVector& Current, const FVector& Target, float MaxDistanceDelta)
{
	// Calculate the difference vector
	FVector Delta = Target - Current;
    
	// Get the distance to the target
	float Distance = Delta.Size();
    
	// If the distance is less than or equal to the allowed delta, return the target
	if (Distance <= MaxDistanceDelta || Distance == 0.f)
	{
		return Target;
	}
    
	// Normalize the delta vector and move by MaxDistanceDelta
	return Current + Delta / Distance * MaxDistanceDelta;
}

// Called every frame
void AMapReinforcementUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && Target)
	{
		AOverworldMap* associatedMap = GetMapTransformComponent()->GetAssociatedMap();
		if (associatedMap)
		{
			IMapObject* mapObjectInterface = Cast<IMapObject>(Target->GetOwner());
			if (mapObjectInterface)
			{
				FVector loc = mapObjectInterface->GetMapTransformComponent()->MapPosition;

				GetMapTransformComponent()->SetMapPosition(MoveTowards(GetMapTransformComponent()->MapPosition, loc, DeltaTime * TransferSpeed));

				if (FVector::Dist(loc, GetMapTransformComponent()->MapPosition) < 0.01)
				{
					Target->OnReceiveTransfer(this);
				}
			}else
			{
				UE_LOG(LogTemp, Error, TEXT("AMapReinforcementUnit::Tick, could not get map object interface from target owner."));
			}

		}
		
		
	}

}

UMapTransformComponent* AMapReinforcementUnit::GetMapTransformComponent()
{
	return MapTransformComponent;
}

void AMapReinforcementUnit::OnTargetChange_Implementation(const UTokenConsumer* newTarget)
{
	Target = newTarget;
}

