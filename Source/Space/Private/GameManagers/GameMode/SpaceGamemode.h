// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Map/MapGenerationStructs.h"
#include "SpaceGamemode.generated.h"

class AOverworldMap;
class AMapPlayer;
class UMapGameModeState;
class UGameModeStateMachine;
/**
 * 
 */


UCLASS()
class ASpaceGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASpaceGamemode();
	virtual void BeginPlay() override;
	FOverworldMapData& GetMapData();
	AOverworldMap* GetOverworldMap();
protected:
	void GenerateOverworldMap(const FWorldGenerationParams& params);
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMapPlayer> MapPlayerClass;
private:
	FOverworldMapData OverworldMap;

	UPROPERTY()
	TObjectPtr<AOverworldMap> OverworldMapActor;
};
