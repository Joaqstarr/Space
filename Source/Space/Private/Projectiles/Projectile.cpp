// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	SphereHitbox = CreateDefaultSubobject<USphereComponent>(FName(TEXT("Sphere Collision")));
	SetRootComponent(SphereHitbox);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName(TEXT("Projectile Movement")));
}



// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::Activate_Implementation()
{
	IPoolableInterface::Activate_Implementation();
	
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void AProjectile::Deactivate_Implementation()
{
	IPoolableInterface::Deactivate_Implementation();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AProjectile::Reset_Implementation()
{
	IPoolableInterface::Reset_Implementation();

	
}

bool AProjectile::IsInActive_Implementation()
{
	return !GetActorEnableCollision() && IsHidden();
}




