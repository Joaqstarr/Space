// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Physics/GravityVolume.h"
#include "BoxGravityVolume.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class UBoxGravityVolume : public UGravityVolume
{
	GENERATED_BODY()
public:
	UBoxGravityVolume();

	virtual FVector GetGravityDirection(const FVector& objectPos) const override;
private:
	TObjectPtr<UBoxComponent> AsBoxComponent;
};
