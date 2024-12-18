// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FactoryPayload.h"
#include "UObject/NoExportTypes.h"
#include "LambdaFactoryPayload.generated.h"

/**
 * 
 */
UCLASS()
class ULambdaFactoryPayload : public UObject, public IFactoryPayload
{
	GENERATED_BODY()
	
public:
	void Bind(TFunction<UObject* ()> Func);
	virtual UObject* CreateInstance_Implementation() const override;
private:
	TFunction<UObject* ()> FactoryFunction;
};
