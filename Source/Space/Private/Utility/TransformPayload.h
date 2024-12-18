// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TransformPayload.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UTransformPayload : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Payload")
	FTransform TransformData;
};
