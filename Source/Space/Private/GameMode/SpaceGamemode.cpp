// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceGamemode.h"
#include "PhysicsEngine/PhysicsSettings.h"
#include "States/GameModeStateMachine.h"
#include "States/MapGameModeState.h"

ASpaceGamemode::ASpaceGamemode()
{
	StateMachineClass = UGameModeStateMachine::StaticClass();
}

void ASpaceGamemode::SwitchToCombatZoneState()
{
	if (!StateMachine)
	{
		UE_LOG(LogTemp, Warning, TEXT("State machine is null"));
		return;
	}
	StateMachine->SwitchToCombatZoneState();
}

void ASpaceGamemode::SwitchToMapState()
{
	if (!StateMachine)
	{
		UE_LOG(LogTemp, Warning, TEXT("State machine is null"));
		return;
	}
	StateMachine->SwitchToMapState();
}

void ASpaceGamemode::StartPlay()
{
	Super::StartPlay();

	StateMachine = NewObject<UGameModeStateMachine>(this, StateMachineClass);
	StateMachine->InitializeStateMachine(nullptr);
	
}
