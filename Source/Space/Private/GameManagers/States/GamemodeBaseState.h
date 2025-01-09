// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/StateMachine/StateMachine.h"
#include "GamemodeBaseState.generated.h"

class ASpacePlayerController;
class ASpaceGameState;
class UGameModeStateMachine;

USTRUCT()
struct FGamemodeStateParams : public FStateMachineInitializationParams
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UGameModeStateMachine> GameModeStateMachine;
	UPROPERTY()
	TObjectPtr<ASpaceGameState> GameState;
	UPROPERTY()
	TWeakObjectPtr<ASpacePlayerController> LocalPlayerController;
};
/**
 * 
 */
UCLASS()
class UGamemodeBaseState : public UStateMachine
{
	GENERATED_BODY()
public:
	virtual void InitializeStateMachine(const TObjectPtr<UStateMachine> DefaultState, const FStateMachineInitializationParams& initializationParams) override;
	virtual void OnEnterState_Implementation() override;
	virtual void OnExitState_Implementation() override;
protected:
	bool HasAuthority() const;

protected:
	UPROPERTY()
	TObjectPtr<UGameModeStateMachine> GameModeStateMachine;
	UPROPERTY()
	TObjectPtr<ASpaceGameState> GameState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> DefaultWidgetClass;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UUserWidget> DefaultWidget;
};
