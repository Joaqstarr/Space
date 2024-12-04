// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpacePawn.h"
#include "Ship.generated.h"


class UDashSet;
class UHealthSet;
class UGameplayAbility;
class UHealthComponent;
class UShipStats;
class UGravityComponent;
class UHoverComponent;
class USkeletalMeshComponent;
class UShipAbilitySystemComponent;

UCLASS()
class AShip : public ASpacePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShip();

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
	
	UFUNCTION(BlueprintCallable)
	void SetAutoLookAtTarget(USceneComponent* targ){TargetLookLocation = targ; IsRotatingToLookAtTarget = true;}
protected:
	UFUNCTION(BlueprintCallable)
	void TryDash(FVector inputDirection);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> ShipMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UGravityComponent> GravityComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHoverComponent> HoverComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UShipStats> ShipStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(BlueprintReadOnly)
	UDashSet* DashAttributeSet;
private:
	float LastYawSpeed {0};
	float ApplyBrakes(FVector movementDir) const;
	void AddTorqueControlAroundAxis(float inputAmount, const FVector& axis, float maxSpeed, float strength, float damp) const;
	void ApplyMovementForce(const FVector& direction, float inputValue, float maxSpeed, float forceScale) const;
	void RotateToFaceLocationPhysics(FVector targetLocation, float torqueStrength, float deltaTime);

private:
	UPROPERTY()
	USceneComponent* TargetLookLocation;
	bool IsRotatingToLookAtTarget {false};

};
