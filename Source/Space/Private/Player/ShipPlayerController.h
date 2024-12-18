// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpacePlayerController.h"
#include "GameFramework/PlayerController.h"
#include "ShipPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AShipPlayerController : public ASpacePlayerController
{
	GENERATED_BODY()
public:
	virtual void AcknowledgePossession(class APawn* p) override;
};
