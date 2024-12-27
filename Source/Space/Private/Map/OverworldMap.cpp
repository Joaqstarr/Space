// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/OverworldMap.h"

#include "MapObject.h"
#include "MapPlanet.h"
#include "MapTransformComponent.h"
#include "Components/BoxComponent.h"
#include "GameManagers/GameMode/SpaceGamemode.h"

// Sets default values
AOverworldMap::AOverworldMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MapBounds = CreateDefaultSubobject<UBoxComponent>("MapBounds");
	bReplicates = true;

}


void AOverworldMap::SpawnWorldMapActors()
{
	if (HasAuthority())
	{
		ASpaceGamemode* gamemode = Cast<ASpaceGamemode>(GetWorld()->GetAuthGameMode());
		if (gamemode)
		{
			for (FPlanetData planet : gamemode->GetMapData().Planets)
			{
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = this;
				spawnParams.Name = FName(planet.Name);
				AMapPlanet* spawnedPlanet = GetWorld()->SpawnActor<AMapPlanet>(PlanetClass, FVector::Zero(), FRotator::ZeroRotator, spawnParams);
				if (spawnedPlanet)
				{
					spawnedPlanet->SetPlanetData(&planet);
					AddMapObject(spawnedPlanet);
				}
			}
		}
	}
}

void AOverworldMap::IntializeMapObjects()
{
	for (AActor* mapObject : MapObjects)
	{
		if (mapObject && mapObject->GetClass()->ImplementsInterface(UMapObject::StaticClass()))
		{
			IMapObject* mapObjectInter = Cast<IMapObject>(mapObject);
			if (mapObjectInter && mapObjectInter->GetMapTransformComponent())
			{
				mapObjectInter->GetMapTransformComponent()->SetAssociatedMap(this);
			}
		}
	}
}

void AOverworldMap::InitialiZeOverworldMap()
{
	SpawnWorldMapActors();
	
	IntializeMapObjects();
}

// Called when the game starts or when spawned
void AOverworldMap::BeginPlay()
{
	Super::BeginPlay();


	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AOverworldMap::InitialiZeOverworldMap);
}

void AOverworldMap::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	for (int32 i = MapObjects.Num() - 1; i >= 0; --i)
	{
		if (!MapObjects[i]) continue;
		if (!MapObjects[i]->GetClass()->ImplementsInterface(UMapObject::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Removing invalid actor from InterfaceActors array: %s"),
				*GetNameSafe(MapObjects[i]));

			MapObjects[i] = nullptr;
		}else
		{
			IMapObject* mapObjectInter = Cast<IMapObject>(MapObjects[i]);
			if (mapObjectInter && mapObjectInter->GetMapTransformComponent())
			{
				mapObjectInter->GetMapTransformComponent()->UpdateWorldPosition(this);
			}

		}
	}
}

// Called every frame
void AOverworldMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOverworldMap::AddMapObject(AActor* mapObject)
{
	if (mapObject && mapObject->GetClass()->ImplementsInterface(UMapObject::StaticClass()) &&!MapObjects.Contains(mapObject))
	{
		MapObjects.Add(mapObject);
		
		IMapObject* mapObjectInter = Cast<IMapObject>(mapObject);
		if (mapObjectInter && mapObjectInter->GetMapTransformComponent())
		{
			mapObjectInter->GetMapTransformComponent()->SetAssociatedMap(this);
		}
	}
}

void AOverworldMap::RemoveMapObject(AActor* mapObject)
{
	if (mapObject && mapObject->GetClass()->ImplementsInterface(UMapObject::StaticClass()) && MapObjects.Contains(mapObject))
	{
		MapObjects.Remove(mapObject);

		IMapObject* mapObjectInter = Cast<IMapObject>(mapObject);
		if (mapObjectInter && mapObjectInter->GetMapTransformComponent())
		{
			mapObjectInter->GetMapTransformComponent()->SetAssociatedMap(nullptr);
		}
	}
}

FVector AOverworldMap::WorldspaceToMapSpace(const FVector& worldPos) const
{
	FVector localPos = MapBounds->GetComponentTransform().InverseTransformPosition(worldPos);
	FVector bounds = MapBounds->GetScaledBoxExtent();
	localPos.X += bounds.X/2;
	localPos.Y += bounds.Y/2;
	localPos.Z += bounds.Z/2;
	
	return localPos/bounds;
}

FVector AOverworldMap::MapSpaceToWorldSpace(const FVector& mapSpace) const
{
	FVector bounds = MapBounds->GetScaledBoxExtent();
	FVector boxOrigin = MapBounds->GetComponentLocation();

	FVector localPosition = (mapSpace * 2.0f - FVector(1.0f)) * bounds;


	FVector worldPos = localPosition + boxOrigin;
	return worldPos;
}
void AOverworldMap::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOverworldMap, MapObjects);
}