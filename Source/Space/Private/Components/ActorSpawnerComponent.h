// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ActorSpawnerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UActorSpawnerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UActorSpawnerComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorToSpawn;
private:
	void SpawnActor(TSubclassOf<AActor> toSpawn);

		
};
