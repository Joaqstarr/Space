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
