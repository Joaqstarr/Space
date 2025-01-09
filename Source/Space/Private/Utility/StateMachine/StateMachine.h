// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachine.generated.h"

USTRUCT()
struct FStateMachineInitializationParams
{
	GENERATED_BODY()
	
};
UCLASS(Blueprintable)
class SPACE_API UStateMachine: public UObject
{
	GENERATED_BODY()
	
public:
	UStateMachine();
	virtual void InitializeStateMachine(const TObjectPtr<UStateMachine> DefaultState, const FStateMachineInitializationParams& initializationParams);
	
	UFUNCTION(BlueprintNativeEvent)
	void OnEnterState();
	virtual void OnEnterState_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnExitState();
	virtual void OnExitState_Implementation();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnUpdateState(float DeltaTime);
	virtual void OnUpdateState_Implementation(float DeltaTime);
protected:
	void SwitchState(const TObjectPtr<UStateMachine> NewState);
private:
	UPROPERTY()
	TObjectPtr<UStateMachine> CurrentState;
};
