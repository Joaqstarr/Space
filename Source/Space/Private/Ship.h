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
	UFUNCTION(Server, Unreliable)
	void ServerAddRoll(float rollAmount);
	
	UFUNCTION(BlueprintCallable)
	void AddPitch(float pitchAmount);
	UFUNCTION(server, Unreliable)
	void ServerAddPitch(float pitchAmount);
	
	UFUNCTION(BlueprintCallable)
	void AddYaw(float yawAmount);
	UFUNCTION(server, Unreliable)
	void ServerAddYaw(float yawAmount);
	
	UFUNCTION(BlueprintCallable)
	void AddThrust(float forwardThrust, float sidewaysThrust, float verticalThrust);
	UFUNCTION(server, Unreliable)
	void ServerAddThrust(float forwardThrust, float sidewaysThrust, float verticalThrust);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
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

private:
	UPROPERTY(Replicated)
	FVector ReplicatedLocation;

	UPROPERTY(Replicated)
	FRotator ReplicatedRotation;
	UPROPERTY(Replicated)
	FVector ReplicatedVel;

};
