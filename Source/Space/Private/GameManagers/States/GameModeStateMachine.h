// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utility/StateMachine/StateMachine.h"
#include "GameModeStateMachine.generated.h"

class UCombatZoneGameModeState;
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
	virtual void InitializeStateMachine(const TObjectPtr<UStateMachine> DefaultState, const FStateMachineInitializationParams& initializationParams) override;
	UFUNCTION(BlueprintCallable)
	void SwitchToCombatZoneState();
	UFUNCTION(BlueprintCallable)
	void SwitchToMapState();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="States")
	TSubclassOf<UMapGameModeState> MapGameModeStateClass;
	UPROPERTY()
	TObjectPtr<UMapGameModeState> MapGameModeState;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="States")
	TSubclassOf<UCombatZoneGameModeState> CombatZoneGameModeStateClass;
	UPROPERTY()
	TObjectPtr<UCombatZoneGameModeState> CombatZoneGameModeState;
};
