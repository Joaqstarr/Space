// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ShipParts/GunComponent.h"

#include "Components/ObjectPooling/PoolableInterface.h"
#include "Components/ObjectPooling/PoolManagerComponent.h"
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
	
}


// Called every frame
void UGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	fireTimer -= DeltaTime;

	// ...
}

void UGunComponent::Fire(UTargetableComponent* lockedOnTarget)
{
	if(fireTimer > 0)return;
	if(!ProjectilePool)return;
	
	AActor* projectile =  ProjectilePool->GetPooledActor();
	if(!projectile)return;

	fireTimer = 60.f/RoundsPerMinute;
	projectile->SetActorLocation(GetComponentLocation());
	projectile->SetActorRotation(GetComponentRotation());
	
	if(AProjectile* asProjectile = Cast<AProjectile>(projectile)) asProjectile->SetTarget(lockedOnTarget);
	
	if(projectile->Implements<UPoolableInterface>())
	{
		IPoolableInterface::Execute_Reset(projectile);
	}
}

