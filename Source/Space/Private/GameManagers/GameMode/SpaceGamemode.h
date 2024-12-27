// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Map/MapGenerationStructs.h"
#include "SpaceGamemode.generated.h"

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
	virtual void BeginPlay() override;
	FOverworldMapData& GetMapData();
protected:
	void GenerateOverworldMap(const FWorldGenerationParams& params);
private:
	FOverworldMapData OverworldMap;
};
