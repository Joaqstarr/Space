// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/StateMachine/StateMachine.h"
#include "MapGameModeState.generated.h"

/**
 * 
 */
UCLASS()
class UMapGameModeState : public UStateMachine
{
	GENERATED_BODY()
public:
	virtual void OnEnterState_Implementation() override;
};
