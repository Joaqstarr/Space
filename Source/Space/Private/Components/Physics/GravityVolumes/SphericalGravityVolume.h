// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Physics/GravityVolume.h"
#include "SphericalGravityVolume.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class USphericalGravityVolume : public UGravityVolume
{
	GENERATED_BODY()
public:
	USphericalGravityVolume();

	virtual FVector GetGravityDirection(const FVector& objectPos) const override;

private:
	UPROPERTY()
	TObjectPtr<USphereComponent> AsSphereComponent;
};
