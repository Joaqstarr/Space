// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MapPlanet.h"

#include "MapTransformComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMapPlanet::AMapPlanet()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	PlanetMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PlanetMesh"));
	SetRootComponent(PlanetMesh);

	MapTransformComponent = CreateDefaultSubobject<UMapTransformComponent>(FName("MapTransformComponent"));
	MapTransformComponent->SetIsReplicated(true);
}

UMapTransformComponent* AMapPlanet::GetMapTransformComponent()
{
	return MapTransformComponent;
}


void AMapPlanet::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
}

// Called every frame
void AMapPlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

