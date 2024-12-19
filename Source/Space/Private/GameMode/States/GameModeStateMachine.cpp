// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/States/GameModeStateMachine.h"

#include "CombatZoneGameModeState.h"
#include "MapGameModeState.h"


UGameModeStateMachine::UGameModeStateMachine()
{
	MapGameModeStateClass = UMapGameModeState::StaticClass();
	CombatZoneGameModeStateClass = UCombatZoneGameModeState::StaticClass();
}

void UGameModeStateMachine::OnEnterState_Implementation()
{
}

void UGameModeStateMachine::InitializeStateMachine(const TObjectPtr<UStateMachine> DefaultState)
{
	MapGameModeState = NewObject<UMapGameModeState>(this, MapGameModeStateClass);
	CombatZoneGameModeState = NewObject<UCombatZoneGameModeState>(this, CombatZoneGameModeStateClass);
	
	Super::InitializeStateMachine(MapGameModeState);
}

void UGameModeStateMachine::SwitchToMapState()
{
	SwitchState(MapGameModeState);
}

void UGameModeStateMachine::SwitchToCombatZoneState()
{
	SwitchState(CombatZoneGameModeState);
}
