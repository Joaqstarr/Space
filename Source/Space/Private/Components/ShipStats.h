// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShipStats.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACE_API UShipStats : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShipStats();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ForwardSpeed {100};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float StrafeSpeed {50};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PitchSpeed {5};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RollSpeed {5};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float YawSpeed {5};
};
