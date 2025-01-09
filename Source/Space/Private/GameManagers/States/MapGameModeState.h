// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamemodeBaseState.h"
#include "MapGameModeState.generated.h"

class AMapPlayer;
class AOverworldMap;

UCLASS()
class UMapGameModeState : public UGamemodeBaseState
{
	GENERATED_BODY()
public:
	UMapGameModeState();
	virtual void OnEnterState_Implementation() override;
	virtual void OnExitState_Implementation() override;
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMapPlayer> MapPlayerClass;
private:
	UPROPERTY()
	TObjectPtr<AMapPlayer> MapPlayerPawn;
	UPROPERTY()
	TObjectPtr<AOverworldMap> OverworldMapActor;
};
