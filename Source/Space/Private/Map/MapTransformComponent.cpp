// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MapTransformComponent.h"

#include "OverworldMap.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UMapTransformComponent::UMapTransformComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void UMapTransformComponent::SetAssociatedMap(AOverworldMap* newMap)
{
	AssociatedMap = newMap;
}


// Called when the game starts
void UMapTransformComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UMapTransformComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	UpdateWorldPosition(AssociatedMap);
}

void UMapTransformComponent::OnRep_MapPosition()
{
	UpdateWorldPosition(AssociatedMap);
}


void UMapTransformComponent::UpdateWorldPosition(AOverworldMap* withMap)
{
	AssociatedMap = withMap;
	if (withMap)
	{
		GetOwner()->SetActorLocation(withMap->MapSpaceToWorldSpace(MapPosition));
	}
}

// Called every frame
void UMapTransformComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (GetOwner()->HasAuthority())
	{
		UpdateWorldPosition(AssociatedMap);
	}
}

void UMapTransformComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMapTransformComponent, MapPosition);
	DOREPLIFETIME(UMapTransformComponent, AssociatedMap);

}

