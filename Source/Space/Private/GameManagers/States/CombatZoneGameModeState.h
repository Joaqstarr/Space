// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/StateMachine/StateMachine.h"
#include "CombatZoneGameModeState.generated.h"

/**
 * 
 */
UCLASS()
class UCombatZoneGameModeState : public UStateMachine
{
	GENERATED_BODY()

public:
	virtual void OnEnterState_Implementation() override;
	virtual void OnExitState_Implementation() override;
protected:
	
};
