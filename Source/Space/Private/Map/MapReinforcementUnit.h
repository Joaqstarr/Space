// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapObject.h"
#include "GameFramework/Actor.h"
#include "Utility/TokenTransferUnit.h"
#include "MapReinforcementUnit.generated.h"

UCLASS()
class AMapReinforcementUnit : public AActor, public IMapObject, public ITokenTransferUnit
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapReinforcementUnit();
	virtual void Tick(float DeltaTime) override;
	virtual UMapTransformComponent* GetMapTransformComponent() override;

	virtual void OnTargetChange_Implementation(const UTokenConsumer* NewTarget) override;
	virtual void OnTransferComplete_Implementation() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TransferSpeed = 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UMapTransformComponent> MapTransformComponent;
private:
	UPROPERTY()
	TObjectPtr<const UTokenConsumer> Target;
};
