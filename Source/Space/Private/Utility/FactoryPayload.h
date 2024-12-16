// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FactoryPayload.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UFactoryPayload : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IFactoryPayload
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UObject* CreateInstance() const;
};
