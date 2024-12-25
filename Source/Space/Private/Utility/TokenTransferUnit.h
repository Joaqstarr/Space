// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TokenTransferUnit.generated.h"

class UTokenConsumer;

UINTERFACE(MinimalAPI, Blueprintable)
class UTokenTransferUnit : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ITokenTransferUnit
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnTargetChange(const UTokenConsumer* NewTarget);

	UFUNCTION(BlueprintNativeEvent)
	void OnTransferComplete();
};
