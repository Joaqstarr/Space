// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/StateMachine/StateMachine.h"
#include "GameModeStateMachine.generated.h"

class UMapGameModeState;
/**
 * 
 */
UCLASS()
class UGameModeStateMachine : public UStateMachine
{
	GENERATED_BODY()
public:
	UGameModeStateMachine();
	virtual void OnEnterState_Implementation() override;
	/*Default state ignored, leave as nullptr*/
	virtual void InitializeStateMachine(const TObjectPtr<UStateMachine> DefaultState) override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="States")
	TSubclassOf<UMapGameModeState> MapGameModeStateClass;
	UPROPERTY()
	TObjectPtr<UMapGameModeState> MapGameModeState;
};
