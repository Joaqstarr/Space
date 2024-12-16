// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ship.h"
#include "PlayerShip.generated.h"

class ASword;
class UGameplayAbility;
class UPlayerShipAbilitySystemComponent;
class ATargetable;
class UWidgetComponent;
class UAIPerceptionStimuliSourceComponent;
class UTargetingHandlerComponent;
class USkeletalMeshComponent;
class UPawnMotionWarpingComponent;

USTRUCT(Blueprintable)
struct FShipInputState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	float ForwardThrust;

	UPROPERTY(BlueprintReadWrite)
	float SidewaysThrust;

	UPROPERTY(BlueprintReadWrite)
	float VerticalThrust;
	
	UPROPERTY(BlueprintReadWrite)
	float PitchInput;
	
	UPROPERTY(BlueprintReadWrite)
	float YawInput;
	
	UPROPERTY(BlueprintReadWrite)
	float RollInput;

	FShipInputState()
		: ForwardThrust(0), SidewaysThrust(0), VerticalThrust(0),
		  PitchInput(0), YawInput(0), RollInput(0) {}
	
	FShipInputState(float forward, float sideways, float vertical, float pitch, float yaw, float roll)
		: ForwardThrust(forward), SidewaysThrust(sideways), VerticalThrust(vertical), PitchInput(pitch), YawInput(yaw), RollInput(roll) {}
};

UCLASS()
class APlayerShip : public AShip
{
	GENERATED_BODY()
public:
	APlayerShip();
	UFUNCTION(BlueprintCallable)
	FVector2D GetMousePos(bool affectedByDeadzone) const;

	//returns nullptr if no target
	ATargetable* GetCurrentTarget();

	UPawnMotionWarpingComponent* GetMotionWarping();

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	FVector2D UpdateMousePos(const FVector2D& delta);
	UFUNCTION(BlueprintCallable)
	FVector2D UpdateStickLook(const FVector2D& stickLookInput);
	
	UFUNCTION(BlueprintCallable)
	void ApplyInput(FShipInputState inputState);

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void ServerSendInput(FShipInputState inputState);
protected:
	FVector2D MousePos{};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTargetingHandlerComponent> TargetingManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSource;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> TargetLockIndicator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Weapons)
	TSubclassOf<ASword> SwordClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ASword> SwordActor;

	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UPawnMotionWarpingComponent> PawnMotionWarpingComponent;
	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	float MouseRadius{50};
	UPROPERTY(EditDefaultsOnly, Category=Input, meta=(ClampMin="0.0", ClampMax="1.0", UIMin="0.0", UIMax="1.0"))
	float MouseDeadzone{0.25f};


private:
	UPROPERTY(EditDefaultsOnly, Category=Widgets)
	TObjectPtr<UUserWidget> TargetLockWidget;
private:
	void SetupSwordActor();

};
