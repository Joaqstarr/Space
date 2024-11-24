// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TargetableComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values for this component's properties
UTargetableComponent::UTargetableComponent(const FObjectInitializer& OI) : Super(OI)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	CollisionSphere = OI.CreateDefaultSubobject<USphereComponent>(this, FName("Distance Check Sphere"));
	CollisionSphere->SetCollisionObjectType(ECC_GameTraceChannel1);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionSphere->SetSphereRadius(10);
	CollisionSphere->SetupAttachment(this);

	TargetIndicator = OI.CreateDefaultSubobject<UWidgetComponent>(this, FName("Target Indicator UI"));
	TargetIndicator->SetupAttachment(CollisionSphere);
	TargetIndicator->SetWidgetSpace(EWidgetSpace::Screen);

	
}
void UTargetableComponent::SetupInitialAttachment(USceneComponent* root)
{
	
	SetupAttachment(root);
	CollisionSphere->SetupAttachment(this);
	
	TargetIndicator->SetupAttachment(CollisionSphere);
}
bool UTargetableComponent::IsTargetable() const
{
	return bIsTargetable;
}


void UTargetableComponent::SetTargetable(bool bTargetable)
{
	bIsTargetable = bTargetable;
}

void UTargetableComponent::EnteredRange()
{
	TargetIndicator->SetVisibility(true);

}

void UTargetableComponent::ExitedRange()
{
	TargetIndicator->SetVisibility(false);
}

FVector2D UTargetableComponent::GetScreenPosition() const
{
	if(!bIsOnScreen) return FVector2D(-1,-1);
	return ScreenPosition;
}

void UTargetableComponent::SetScreenPosition(const FVector2D& screenPosition, const bool bOnScreen)
{
	FVector2D viewPortSize;
	GetWorld()->GetGameViewport()->GetViewportSize(viewPortSize);
	
	ScreenPosition = screenPosition/viewPortSize;
	bIsOnScreen = bOnScreen;
}



void UTargetableComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Force subcomponents to update their transforms
	CollisionSphere->UpdateComponentToWorld();
	TargetIndicator->UpdateComponentToWorld();
	
	UE_LOG(LogTemp, Warning, TEXT("Targetable Component Location: %s"), *GetComponentLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("CollisionSphere Location: %s"), *CollisionSphere->GetComponentLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Collision parent: %s"), *CollisionSphere->GetOuter()->GetName());

}
