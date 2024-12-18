// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ShipParts/GunComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "Actors/Targetable.h"
#include "Utility/LambdaFactoryPayload.h"
#include "Utility/TransformPayload.h"
#include "Components/ObjectPooling/PoolableInterface.h"
#include "Components/ObjectPooling/PoolManagerComponent.h"
#include "Components/Player/TargetingHandlerComponent.h"
#include "GameplayEffects/DamageEffect.h"
#include "Projectiles/Projectile.h"

// Sets default values for this component's properties
UGunComponent::UGunComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGunComponent::BeginPlay()
{
	Super::BeginPlay();

	IAbilitySystemInterface* ownerASC = Cast<IAbilitySystemInterface>(GetOwner());
	if (ownerASC)
	{
		AbilitySystemComponent = ownerASC->GetAbilitySystemComponent();
	}
}

void UGunComponent::ServerFire_Implementation(ATargetable* Target)
{
	FGameplayEventData EventData;
	EventData.EventTag = AbilityTag;
	EventData.Instigator = GetOwner();
	EventData.Target = Target;

	ULambdaFactoryPayload* GetProjectilePayload = NewObject<ULambdaFactoryPayload>();
	GetProjectilePayload->Bind([&]()
		{
			return ProjectilePool->GetPooledActor();
		}
	);
	
	EventData.OptionalObject = GetProjectilePayload;

	UTransformPayload* SpawnTransformPayload = NewObject<UTransformPayload>();
	SpawnTransformPayload->TransformData = GetComponentTransform();
	EventData.OptionalObject2 = SpawnTransformPayload;

	AbilitySystemComponent->HandleGameplayEvent(EventData.EventTag, &EventData);
}

