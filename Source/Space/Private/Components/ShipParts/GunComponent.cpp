// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ShipParts/GunComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "Actors/Targetable.h"
#include "Components/ObjectPooling/PoolableInterface.h"
#include "Components/ObjectPooling/PoolManagerComponent.h"
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

	// ...
	IAbilitySystemInterface* ownerASC = Cast<IAbilitySystemInterface>(GetOwner());
	if (ownerASC)
	{
		AbilitySystemComponent = ownerASC->GetAbilitySystemComponent();
	}
}


// Called every frame
void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	fireTimer -= DeltaTime;

	// ...
}

void UGunComponent::Fire(ATargetable* lockedOnTarget)
{
	if(fireTimer > 0)return;
	if(!ProjectilePool)return;
	
	AActor* projectile =  ProjectilePool->GetPooledActor();
	if(!projectile)return;

	fireTimer = 60.f/RoundsPerMinute;
	projectile->SetActorLocation(GetComponentLocation());
	projectile->SetActorRotation(GetComponentRotation());
	
	if(AProjectile* asProjectile = Cast<AProjectile>(projectile))
	{
		asProjectile->SetTarget(lockedOnTarget);

		//UE_LOG(LogTemp, Display, TEXT("fire"));

		FInitializeProjectileParams projectileParams;
		projectileParams.Damage = Damage;
		projectileParams.InstigatorActor = GetOwner();
		
		if(AbilitySystemComponent && OptionalGameplayEffectClass)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddInstigator(GetOwner(), GetOwner());

			projectileParams.OptionalAdditionalEffect = AbilitySystemComponent->MakeOutgoingSpec(OptionalGameplayEffectClass, 1.0f, EffectContext);
		}

		asProjectile->InitializeProjectile(projectileParams);

	}
	
	if(projectile->Implements<UPoolableInterface>())
	{
		IPoolableInterface::Execute_Reset(projectile);
	}
}

