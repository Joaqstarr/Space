// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagers/States/GamemodeBaseState.h"

#include "Blueprint/UserWidget.h"
#include "GameManagers/SpaceGameState.h"

void UGamemodeBaseState::InitializeStateMachine(const TObjectPtr<UStateMachine> DefaultState,
                                                const FStateMachineInitializationParams& initializationParams)
{
	Super::InitializeStateMachine(DefaultState, initializationParams);
	
	try
	{
		const FGamemodeStateParams& asGamemodeStateParams = static_cast<const FGamemodeStateParams&>(initializationParams);

		GameModeStateMachine = asGamemodeStateParams.GameModeStateMachine;
		GameState = asGamemodeStateParams.GameState;
		
	}catch (const char* msg)
	{
		UE_LOG(LogTemp, Error, TEXT("%hs"), msg);
	}
	
}

void UGamemodeBaseState::OnEnterState_Implementation()
{
	
}

void UGamemodeBaseState::OnExitState_Implementation()
{
	
}

bool UGamemodeBaseState::HasAuthority() const
{
	if (GameState)
	{
		return GameState->HasAuthority();
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), TEXT("Gamestate ptr is null."));
		return false;
	}
}


