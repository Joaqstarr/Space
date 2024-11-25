// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/Projectile.h"
#include "../Components/TargetableComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SphereHitbox = CreateDefaultSubobject<USphereComponent>(FName(TEXT("Sphere Collision")));
	SetRootComponent(SphereHitbox);

	SphereHitbox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	SphereHitbox->SetCollisionObjectType(ECC_PhysicsBody);
	SphereHitbox->SetCollisionResponseToAllChannels(ECR_Block);

	SphereHitbox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	SphereHitbox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	SphereHitbox->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);

	

	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName(TEXT("Projectile Movement")));
	ProjectileMovementComponent->SetUpdatedComponent(SphereHitbox);
	ProjectileMovementComponent->bAutoActivate = false;
	ProjectileMovementComponent->InitialSpeed = MaxSpeed;
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	SetCanBeDamaged(false);
}



// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovementComponent->OnComponentActivated.AddDynamic(this, &AProjectile::OnProjectileMCActivated);
	
}

void AProjectile::OnProjectileMCActivated( UActorComponent* Component, bool bReset)
{
	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector::ForwardVector * MaxSpeed);
}

void AProjectile::DisableHomingOffAngle(float minDot)
{
	if(ProjectileMovementComponent->HomingTargetComponent == nullptr)return;

	FVector dir = (ProjectileMovementComponent->HomingTargetComponent->GetComponentLocation() - GetActorLocation()).GetSafeNormal();
	float dot = FVector::DotProduct(GetActorForwardVector(), dir);

	if(dot < minDot)
	{
		ProjectileMovementComponent->HomingTargetComponent = nullptr;
		ProjectileMovementComponent->bIsHomingProjectile = false;
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DisableHomingOffAngle(0.4f);

	
}

void AProjectile::Activate_Implementation()
{
	IPoolableInterface::Activate_Implementation();

	if(UPrimitiveComponent* asPrimitive = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		asPrimitive->IgnoreActorWhenMoving(GetOwner(), true);
	}

	//ProjectileMovementComponent->SetComponentTickEnabled(true);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void AProjectile::Deactivate_Implementation()
{
	IPoolableInterface::Deactivate_Implementation();

	if(UPrimitiveComponent* asPrimitive = Cast<UPrimitiveComponent>(GetRootComponent()))
	{
		asPrimitive->IgnoreActorWhenMoving(GetOwner(), false);
	}
	GetWorldTimerManager().ClearTimer(DespawnTimerHandle);
	
	ProjectileMovementComponent->Deactivate();
	//ProjectileMovementComponent->SetComponentTickEnabled(false);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void AProjectile::Reset_Implementation()
{	
	IPoolableInterface::Reset_Implementation();
	ProjectileMovementComponent->Activate(true);
	

	auto TimerDelegate = FTimerDelegate::CreateUObject(this, &AProjectile::Deactivate_Implementation);

	GetWorldTimerManager().SetTimer(DespawnTimerHandle, TimerDelegate, LifeTime, false);
	
}


bool AProjectile::IsInactive_Implementation()
{
	return !GetActorEnableCollision() && IsHidden();
}

void AProjectile::SetTarget(UTargetableComponent* target)
{
	//USceneComponent* asScene = Cast<USceneComponent>(target);
	ProjectileMovementComponent->bIsHomingProjectile = (target != nullptr);
	ProjectileMovementComponent->HomingTargetComponent = target;
}




