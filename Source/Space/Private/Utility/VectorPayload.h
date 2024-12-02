#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VectorPayload.generated.h"

/**
 * A simple UObject wrapper for FVector
 */
UCLASS(BlueprintType)
class UVectorPayload : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "Payload")
	FVector VectorData;
};