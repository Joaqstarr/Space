// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpacePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ASpacePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void AcknowledgePossession(class APawn* p) override;
};
