// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Targetable.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
// Sets default values
ATargetable::ATargetable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("Distance Check Sphere"));
	CollisionSphere->SetCollisionObjectType(ECC_GameTraceChannel1);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionSphere->SetSphereRadius(10);
	SetRootComponent(CollisionSphere);

	TargetIndicator = CreateDefaultSubobject<UWidgetComponent>(FName("Target Indicator UI"));
	TargetIndicator->SetupAttachment(CollisionSphere);
	TargetIndicator->SetWidgetSpace(EWidgetSpace::Screen);
	TargetIndicator->SetVisibility(true);
}



bool ATargetable::IsTargetable() const
{
	return bIsTargetable;
}


void ATargetable::SetTargetable(bool bTargetable)
{
	bIsTargetable = bTargetable;
}

void ATargetable::EnteredRange()
{
	TargetIndicator->SetVisibility(true);

}

void ATargetable::ExitedRange()
{
	TargetIndicator->SetVisibility(false);
}

FVector2D ATargetable::GetScreenPosition() const
{
	if(!bIsOnScreen) return FVector2D(-1,-1);
	return ScreenPosition;
}

void ATargetable::SetScreenPosition(const FVector2D& screenPosition, const bool bOnScreen)
{
	FVector2D viewPortSize;
	GetWorld()->GetGameViewport()->GetViewportSize(viewPortSize);
	
	ScreenPosition = screenPosition/viewPortSize;
	bIsOnScreen = bOnScreen;
}


