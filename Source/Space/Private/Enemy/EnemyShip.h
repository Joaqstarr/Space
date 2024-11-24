// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "EnemyShip.generated.h"

class UTargetableComponent;
/**
 * 
 */
UCLASS()
class AEnemyShip : public AShip
{
	GENERATED_BODY()
public:
	AEnemyShip(const FObjectInitializer& OI);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTargetableComponent> TargetableComponent;
};
