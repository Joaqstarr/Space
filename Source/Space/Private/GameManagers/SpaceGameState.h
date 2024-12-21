// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SpaceGameState.generated.h"

class UGameModeStateMachine;
/**
 * 
 */
UCLASS()
class ASpaceGameState : public AGameStateBase
{
public:
	ASpaceGameState();
	GENERATED_BODY()

	/*
	 * Switch state to combat zone. Must be called on server
	 */
	UFUNCTION(BlueprintCallable)
	void SwitchToCombatZoneState();
	/*
	 * Switch state to map. Must be called on server
	 */
	UFUNCTION(BlueprintCallable)
	void SwitchToMapState();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSwitchToCombatZoneState();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSwitchToMapState();
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="States")
	TSubclassOf<UGameModeStateMachine> StateMachineClass;
	
	UPROPERTY()
	TObjectPtr<UGameModeStateMachine> StateMachine;
};
