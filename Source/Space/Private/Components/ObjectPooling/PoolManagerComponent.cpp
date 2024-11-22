// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ObjectPooling/PoolManagerComponent.h"
#include "PoolableInterface.h"

// Sets default values for this component's properties
UPoolManagerComponent::UPoolManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UPoolManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializePool();
}

void UPoolManagerComponent::InitializePool()
{
	if(!ClassToPool)return;
	FName folderPath(GetOwner()->GetName() + "_Pools/" + GetName());
	
	for(int i = 0; i < PoolSize; i++)
	{
		AActor* spawnedActor = GetWorld()->SpawnActor<AActor>(ClassToPool);
		
		if(spawnedActor && spawnedActor->Implements<UPoolableInterface>())
		{
			spawnedActor->SetFolderPath(folderPath);
			IPoolableInterface::Execute_Deactivate(spawnedActor);
			Pool.Add(spawnedActor);
		}else
		{
			GetWorld()->DestroyActor(spawnedActor);
		}
	}
}


AActor* UPoolManagerComponent::GetPooledActor()
{
	for(AActor* actor : Pool)
	{
		if(IPoolableInterface::Execute_IsInActive(actor))
		{
			IPoolableInterface::Execute_Activate(actor);
			return actor;
		}
	}
	
	return nullptr;
}

void UPoolManagerComponent::ReturnPooledActor(AActor* actor)
{
	if(actor->Implements<UPoolableInterface>())
	{
		IPoolableInterface::Execute_Deactivate(actor);
	}
}


