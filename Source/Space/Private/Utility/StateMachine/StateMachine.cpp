// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"


UStateMachine::UStateMachine()
{
	CurrentState = nullptr;
}

void UStateMachine::InitializeStateMachine(const TObjectPtr<UStateMachine> DefaultState)
{
	SwitchState(DefaultState);
}

void UStateMachine::OnEnterState_Implementation()
PURE_VIRTUAL(UStateMachine::OnEnterState_Implementation,)

void UStateMachine::OnExitState_Implementation()
PURE_VIRTUAL(UStateMachine::OnExitState_Implementation,)

void UStateMachine::OnUpdateState_Implementation(float DeltaTime)
{
	if(CurrentState != nullptr)
		CurrentState->OnUpdateState(DeltaTime);
}

void UStateMachine::SwitchState(const TObjectPtr<UStateMachine> NewState)
{
	if(CurrentState != nullptr)
		CurrentState->OnExitState();
	
	CurrentState = NewState;
	
	if(CurrentState != nullptr)
		CurrentState->OnEnterState();
}
