// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpacePawn.h"
#include "GameFramework/Pawn.h"
#include "Map/MapObject.h"
#include "MapPlayer.generated.h"

class UMapTransformComponent;

UCLASS()
class AMapPlayer : public ASpacePawn, public IMapObject
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMapPlayer();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UMapTransformComponent* GetMapTransformComponent() override;

	UFUNCTION(BlueprintCallable)
	void TryAddMovementInput(FVector WorldDirection, float ScaleValue);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UMapTransformComponent> MapTransformComponent;

private:
	UFUNCTION(Server, Reliable)
	void AddMapMovementInputServer(FVector WorldDirection, float ScaleValue);
	void AddMapMovementInput(FVector WorldDirection, float ScaleValue = 1);

};
