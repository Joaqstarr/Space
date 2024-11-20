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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement)
	float ForwardSpeed {100};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement)
	float StrafeSpeed {50};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement)
	float ForceScale {1};

	//describes the dot in which the brakes begin to be applied
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement)
	float BrakeStart {0.3f};

	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Turning)
	float PitchSpeed {5};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Turning)
	float RollSpeed {5};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Turning)
	float YawSpeed {5};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Turning)
	float TorqueStrength {5000};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Turning)
	float Damping {0.5f};
};
