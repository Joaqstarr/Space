// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerShip.h"

#include "Components/HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Player/TargetingHandlerComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

APlayerShip::APlayerShip(const FObjectInitializer& OI) : AShip(OI)
{
	TargetingManager = OI.CreateDefaultSubobject<UTargetingHandlerComponent>(this, FName("TargetingHandler"));

	StimuliSource = OI.CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(this, FName("AiStimuliSource"));

	TargetLockIndicator = OI.CreateDefaultSubobject<UWidgetComponent>(this, FName("TargetLockIndicator"));
	TargetLockIndicator->SetWidgetSpace(EWidgetSpace::Screen);
	TargetLockIndicator->SetVisibility(true);
	TargetLockIndicator->SetDrawSize(FVector2D(30,30));
	TargetLockIndicator->SetupAttachment(ShipMeshComponent);
	
}

FVector2D APlayerShip::GetMousePos(bool affectedByDeadzone) const
{
	FVector2D mousePosNorm {MousePos/MouseRadius};

	if(affectedByDeadzone)
	{
		if(mousePosNorm.Length() < MouseDeadzone) return FVector2D::Zero();
		mousePosNorm.X = (mousePosNorm.X - MouseDeadzone)/(1 - MouseDeadzone);
		mousePosNorm.Y = (mousePosNorm.Y - MouseDeadzone)/(1 - MouseDeadzone);
	}
	return mousePosNorm;
}

void APlayerShip::BeginPlay()
{
	Super::BeginPlay();
	TargetLockIndicator->SetWidget(TargetLockWidget);

}

FVector2D APlayerShip::UpdateMousePos(const FVector2D& delta)
{
	MousePos.X += delta.X;
	MousePos.Y += delta.Y;

	if(MousePos.Length() > MouseRadius)
	{
		MousePos.Normalize();
		MousePos = MousePos * MouseRadius;
	}
	return MousePos;
}


