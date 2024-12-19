// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceGamemode.generated.h"

class UMapGameModeState;
class UGameModeStateMachine;
/**
 * 
 */
UCLASS()
class ASpaceGamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASpaceGamemode();
protected:
	virtual void StartPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="States")
	TSubclassOf<UGameModeStateMachine> StateMachineClass;
	
	UPROPERTY()
	TObjectPtr<UGameModeStateMachine> StateMachine;
};
