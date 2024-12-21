
// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagers/SpaceGameState.h"
#include "States/GameModeStateMachine.h"


ASpaceGameState::ASpaceGameState()
{
	StateMachineClass = UGameModeStateMachine::StaticClass();
}

void ASpaceGameState::SwitchToCombatZoneState()
{
	if (HasAuthority())
	{
		MulticastSwitchToCombatZoneState();
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("Switch to combat state must be called on server"));

	}
}

void ASpaceGameState::SwitchToMapState()
{
	if (HasAuthority())
	{
		MulticastSwitchToMapState();
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("Switch to map state must be called on server"));

	}
}

void ASpaceGameState::MulticastSwitchToCombatZoneState_Implementation()
{
	if (!StateMachine)
	{
		UE_LOG(LogTemp, Warning, TEXT("State machine is null"));
		return;
	}

	StateMachine->SwitchToCombatZoneState();
}

void ASpaceGameState::MulticastSwitchToMapState_Implementation()
{
	if (!StateMachine)
	{
		UE_LOG(LogTemp, Warning, TEXT("State machine is null"));
		return;
	}

	StateMachine->SwitchToMapState();
}


void ASpaceGameState::BeginPlay()
{
	Super::BeginPlay();
	StateMachine = NewObject<UGameModeStateMachine>(this, StateMachineClass);
	StateMachine->InitializeStateMachine(nullptr);
}
