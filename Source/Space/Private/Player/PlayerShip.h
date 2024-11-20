// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ship.h"
#include "PlayerShip.generated.h"

class USkeletalMeshComponent;

UCLASS()
class APlayerShip : public AShip
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	FVector2D GetMousePos() const {return MousePos/MouseRadius;};
protected:
	UFUNCTION(BlueprintCallable)
	FVector2D UpdateMousePos(const FVector2D& delta);
	FVector2D MousePos{};

	UPROPERTY(EditDefaultsOnly, Category=Input)
	float MouseRadius{50};

};
