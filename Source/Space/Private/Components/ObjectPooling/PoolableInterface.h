#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolableInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPoolableInterface : public UInterface
{
	GENERATED_BODY()
};

class IPoolableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void Activate();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Deactivate();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Reset();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsInActive();
};
