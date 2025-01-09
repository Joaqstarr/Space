// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagers/States/GameModeStateMachine.h"

#include "CombatZoneGameModeState.h"
#include "MapGameModeState.h"
#include "GameManagers/SpaceGameState.h"


UGameModeStateMachine::UGameModeStateMachine()
{
	MapGameModeStateClass = UMapGameModeState::StaticClass();
	CombatZoneGameModeStateClass = UCombatZoneGameModeState::StaticClass();
}

void UGameModeStateMachine::OnEnterState_Implementation()
{
}



void UGameModeStateMachine::InitializeStateMachine(const TObjectPtr<UStateMachine> DefaultState,
	const FStateMachineInitializationParams& initializationParams)
{
	Super::InitializeStateMachine(DefaultState, initializationParams);
	MapGameModeState = NewObject<UMapGameModeState>(this, MapGameModeStateClass);
	CombatZoneGameModeState = NewObject<UCombatZoneGameModeState>(this, CombatZoneGameModeStateClass);

	ASpaceGameState* gameState = Cast<ASpaceGameState>(GetWorld()->GetGameState());
	
	FGamemodeStateParams initParams;
	initParams.GameModeStateMachine = this;
	initParams.GameState = gameState;
	
	MapGameModeState->InitializeStateMachine(nullptr,initParams);
	CombatZoneGameModeState->InitializeStateMachine(nullptr,initParams);
	
	Super::InitializeStateMachine(MapGameModeState, initializationParams);
}

void UGameModeStateMachine::SwitchToMapState()
{
	SwitchState(MapGameModeState);
}

void UGameModeStateMachine::SwitchToCombatZoneState()
{
	SwitchState(CombatZoneGameModeState);
}
