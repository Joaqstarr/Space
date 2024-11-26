// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TargetableComponent.h"

#include "Actors/Targetable.h"


// Sets default values for this component's properties
UTargetableComponent::UTargetableComponent() : Super()
{
	PrimaryComponentTick.bCanEverTick = false;

	ActorToSpawn = ATargetable::StaticClass();
}
