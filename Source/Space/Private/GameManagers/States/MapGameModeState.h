// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GamemodeBaseState.h"
#include "MapGameModeState.generated.h"

/**
 * 
 */
UCLASS()
class UMapGameModeState : public UGamemodeBaseState
{
	GENERATED_BODY()
public:
	virtual void OnEnterState_Implementation() override;
	virtual void OnExitState_Implementation() override;
};
