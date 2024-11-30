// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerShip.h"
#include "Abilities/PlayerShipAbilitySystemComponent.h"
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

	AbilitySystemComponent = OI.CreateDefaultSubobject<UPlayerShipAbilitySystemComponent>(this, FName("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
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

UAbilitySystemComponent* APlayerShip::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APlayerShip::BeginPlay()
{
	Super::BeginPlay();
	TargetLockIndicator->SetWidget(TargetLockWidget);

	AbilitySystemComponent->InitAbilityActorInfo(GetController(), this);

	GiveDefaultAbilities();

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

void APlayerShip::GiveDefaultAbilities()
{
	check(AbilitySystemComponent);
	if(!HasAuthority()) return;
	
	for(TSubclassOf<UGameplayAbility> abilityClass : DefaultAbilities)
	{
		const FGameplayAbilitySpec abilitySpec(abilityClass, 1);
		
		AbilitySystemComponent->GiveAbility(abilitySpec);
	}
}


