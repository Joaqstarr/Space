// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MapPlanet.h"

#include "MapEnemyBase.h"
#include "MapGenerationStructs.h"
#include "MapTransformComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TokenConsumer.h"

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

void AMapPlanet::SetPlanetData(FPlanetData* planetData, UClass* enemyBaseClass)
{
	PlanetData = planetData;
	MapTransformComponent->MapPosition = planetData->Location;

	for (FBaseData& baseData : planetData->Bases)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Name = FName(baseData.Name);
		AMapEnemyBase* spawnedBase = GetWorld()->SpawnActor<AMapEnemyBase>(enemyBaseClass, FVector::Zero(), FRotator::ZeroRotator, spawnParams);

		if (spawnedBase)
		{
			spawnedBase->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale, NAME_None);
			baseData.TokenConsumer = spawnedBase->GetTokenConsumer();
			baseData.TokenConsumer->SetPriority(5);
		}
	}
}

