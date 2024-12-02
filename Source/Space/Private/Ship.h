// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "Ship.generated.h"


class UHealthSet;
class UGameplayAbility;
class UHealthComponent;
class UShipStats;
class UGravityComponent;
class UHoverComponent;
class USkeletalMeshComponent;
class UShipAbilitySystemComponent;

UCLASS()
class AShip : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShip(const FObjectInitializer& OI);
	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
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
	void GiveDefaultAbilities();
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

	/*
	 *	GAMEPLAY ABILITY SYSTEM
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UShipAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UHealthSet> HealthSet;
private:
	float LastYawSpeed {0};
	float ApplyBrakes(FVector movementDir) const;
	void AddTorqueControlAroundAxis(float inputAmount, const FVector& axis, float maxSpeed, float strength, float damp) const;
	void ApplyMovementForce(const FVector& direction, float inputValue, float maxSpeed, float forceScale) const;

};
