// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/MapEnemyBase.h"

#include "MapReinforcementUnit.h"
#include "MapTransformComponent.h"
#include "Components/TokenConsumer.h"

// Sets default values
AMapEnemyBase::AMapEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TokenConsumer = CreateDefaultSubobject<UTokenConsumer>(FName("TokenConsumer"));
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));
	SetRootComponent(RootSceneComponent);
	bReplicates = true;
	MapReinforcementClass = AMapReinforcementUnit::StaticClass();
}

// Called when the game starts or when spawned
void AMapEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	TokenConsumer->OnTransferSent.AddDynamic(this, &AMapEnemyBase::AMapEnemyBase::SendReinforcements);
	
}

void AMapEnemyBase::SendReinforcements(const UTokenConsumer* target, int tokenAmount)
{
	FActorSpawnParameters spawnParameters;
	spawnParameters.Owner = this;
	
	AMapReinforcementUnit* spawnedUnit = GetWorld()->SpawnActor<AMapReinforcementUnit>(MapReinforcementClass, spawnParameters);

	if (spawnedUnit != nullptr)
	{
		spawnedUnit->GetMapTransformComponent()->SetAssociatedMap(GetMapTransformComponent()->GetAssociatedMap());
		spawnedUnit->GetMapTransformComponent()->SetMapPosition(GetMapTransformComponent()->MapPosition);
		target->RegisterTransfer(spawnedUnit, target, tokenAmount);
	}
}


// Called every frame
void AMapEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UTokenConsumer* AMapEnemyBase::GetTokenConsumer()
{
	return TokenConsumer;
}

UMapTransformComponent* AMapEnemyBase::GetMapTransformComponent()
{
	if (MapTransform == nullptr)
	{
		IMapObject* mapObject = Cast<IMapObject>(GetOwner());
		if (mapObject)
		{
			MapTransform = mapObject->GetMapTransformComponent();
		}

		if (MapTransform == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy Base Not Attached to Map Object Actor."))
		}
	}
	return MapTransform;
}

