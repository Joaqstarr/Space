// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyShip.h"

#include "Components/SphereComponent.h"
#include "Components/TargetableComponent.h"
#include "Components/WidgetComponent.h"

AEnemyShip::AEnemyShip(const FObjectInitializer& OI) : Super(OI)
{
	TargetableComponent = OI.CreateDefaultSubobject<UTargetableComponent>(this, FName("Targetable"));
	if(ShipMesh == nullptr) return;
	
	TargetableComponent->SetupAttachment(ShipMesh);
	TargetableComponent->CollisionSphere->SetupAttachment(ShipMesh);
	
	TargetableComponent->TargetIndicator->SetupAttachment(TargetableComponent->CollisionSphere);
}

