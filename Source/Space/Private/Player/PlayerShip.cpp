// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerShip.h"
#include "Components/WidgetComponent.h"
#include "Components/Player/TargetingHandlerComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

APlayerShip::APlayerShip() : AShip()
{
	TargetingManager = CreateDefaultSubobject<UTargetingHandlerComponent>(FName("TargetingHandler"));

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(FName("AiStimuliSource"));

	TargetLockIndicator = CreateDefaultSubobject<UWidgetComponent>(FName("TargetLockIndicator"));
	TargetLockIndicator->SetWidgetSpace(EWidgetSpace::Screen);
	TargetLockIndicator->SetVisibility(false);
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

ATargetable* APlayerShip::GetCurrentTarget()
{
	if(!TargetingManager)return nullptr;
	return TargetingManager->GetCurrentTarget();
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



