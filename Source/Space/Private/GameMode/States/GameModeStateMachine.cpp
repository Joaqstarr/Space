// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/States/GameModeStateMachine.h"

#include "MapGameModeState.h"


UGameModeStateMachine::UGameModeStateMachine()
{
	MapGameModeStateClass = UMapGameModeState::StaticClass();
}

void UGameModeStateMachine::OnEnterState_Implementation()
{
}

void UGameModeStateMachine::InitializeStateMachine(const TObjectPtr<UStateMachine> DefaultState)
{
	MapGameModeState = NewObject<UMapGameModeState>(this, MapGameModeStateClass);
	
	Super::InitializeStateMachine(MapGameModeState);
}
