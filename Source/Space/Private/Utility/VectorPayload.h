#pragma once

#include "CoreMinimal.h"
#include "VectorPayload.generated.h"

/**
 * A simple UObject wrapper for FVector
 */
UCLASS(BlueprintType)
class UVectorPayload : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "Payload", Replicated)
	FVector VectorData;

	virtual bool IsSupportedForNetworking() const override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

};