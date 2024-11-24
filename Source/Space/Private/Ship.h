// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ship.generated.h"


class UShipStats;
class UGravityComponent;
class UHoverComponent;

UCLASS()
class AShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShip(const FObjectInitializer& OI);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void AddRoll(float rollAmount);

	UFUNCTION(BlueprintCallable)
	void AddPitch(float pitchAmount);

	UFUNCTION(BlueprintCallable)
	void AddYaw(float yawAmount);

	UFUNCTION(BlueprintCallable)
	void AddThrust(float forwardThrust, float sidewaysThrust, float verticalThrust);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ShipMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UGravityComponent* GravityComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UHoverComponent* HoverComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UShipStats* ShipStats;

	

private:
	float LastYawSpeed {0};
	float ApplyBrakes(FVector movementDir) const;
	void AddTorqueControlAroundAxis(float inputAmount, const FVector& axis, float maxSpeed, float strength, float damp) const;
	void ApplyMovementForce(const FVector& direction, float inputValue, float maxSpeed, float forceScale) const;

};
