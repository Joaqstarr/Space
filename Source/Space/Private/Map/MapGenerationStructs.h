#pragma once

#include "CoreMinimal.h"
#include "MapGenerationStructs.generated.h"

class UTokenConsumer;

USTRUCT(BlueprintType)
struct FWorldGenerationParams
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct FBaseData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName Name;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTokenConsumer> TokenConsumer;
};

USTRUCT(BlueprintType)
struct FPlanetData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FName Name;
	
	/* float 0-1
	 *  X: Left - Right
	 *  Y: Back - Forward
	 *  Z: Bottom - Top
	 */
	UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadOnly)
	TArray<struct FBaseData> Bases;
	
};



USTRUCT(BlueprintType)
struct FOverworldMapData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<struct FPlanetData> Planets;
};