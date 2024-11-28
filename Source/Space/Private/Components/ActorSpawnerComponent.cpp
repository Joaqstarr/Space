// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ActorSpawnerComponent.h"


// Sets default values for this component's properties
UActorSpawnerComponent::UActorSpawnerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UActorSpawnerComponent::SpawnActor(TSubclassOf<AActor> toSpawn)
{
	if(toSpawn){
		FActorSpawnParameters spawnParameters;
		spawnParameters.Owner = GetOwner();
		spawnParameters.Instigator = GetOwner()->GetInstigator();
		spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector spawnLocation {GetComponentLocation()};
		FRotator spawnRotation {GetComponentRotation()};

		AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(toSpawn, spawnLocation, spawnRotation, spawnParameters);
		spawnedActor->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);

		SpawnedActor = spawnedActor;

		GetOwner()->OnDestroyed.AddDynamic(this, &UActorSpawnerComponent::DestroySpawnedActor);
	}
}

void UActorSpawnerComponent::DestroySpawnedActor(AActor* actor)
{
	if(SpawnedActor)
	{
		GetWorld()->DestroyActor(SpawnedActor);
	}
}

// Called when the game starts
void UActorSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnActor(ActorToSpawn);
}
