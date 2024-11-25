// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ship.h"
#include "PlayerShip.generated.h"

class UAIPerceptionStimuliSourceComponent;class UTargetingHandlerComponent;
class USkeletalMeshComponent;

UCLASS()
class APlayerShip : public AShip
{
	GENERATED_BODY()
public:
	APlayerShip(const FObjectInitializer& OI);
	UFUNCTION(BlueprintCallable)
	FVector2D GetMousePos(bool affectedByDeadzone) const;
protected:
	UFUNCTION(BlueprintCallable)
	FVector2D UpdateMousePos(const FVector2D& delta);
	FVector2D MousePos{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTargetingHandlerComponent> TargetingManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSource;
	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	float MouseRadius{50};
	UPROPERTY(EditDefaultsOnly, Category=Input, meta=(ClampMin="0.0", ClampMax="1.0", UIMin="0.0", UIMax="1.0"))
	float MouseDeadzone{0.25f};
};
