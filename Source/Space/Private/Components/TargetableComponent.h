// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorSpawnerComponent.h"
#include "Components/SceneComponent.h"
#include "TargetableComponent.generated.h"


class USphereComponent;
class UWidgetComponent;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTargetableComponent : public UActorSpawnerComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetableComponent();
	
};
