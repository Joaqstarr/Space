// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Components/ObjectPooling/PoolableInterface.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

struct FGameplayEffectSpec;
class ATargetable;
class USphereComponent;
class UProjectileMovementComponent;

USTRUCT()
struct FInitializeProjectileParams
{
	GENERATED_BODY()
public:
	float Damage;
	FGameplayEffectSpecHandle OptionalAdditionalEffect;
	UPROPERTY()
	AActor* InstigatorActor;
};

UCLASS()
class AProjectile : public AActor, public IPoolableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	virtual void Tick(float DeltaTime) override;

	virtual void Activate_Implementation() override;
	virtual void Deactivate_Implementation() override;
	virtual void Reset_Implementation() override;
	virtual bool IsInactive_Implementation() override;

	void SetTarget(const ATargetable* target);
	void InitializeProjectile(const FInitializeProjectileParams& initializeProjectileParams);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadOnly)
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile Stats")
	float LifeTime = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile Stats")
	float MaxSpeed = 600;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereHitbox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

private:
	UFUNCTION()
	void OnSphereComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit );
	UPROPERTY()
	FTimerHandle DespawnTimerHandle;

	UFUNCTION()
	void OnProjectileMCActivated( UActorComponent* Component, bool bReset);

	void DisableHomingOffAngle(float minDot);
	
	// Gameplay Effect Spec Handle passed to the projectile
	FGameplayEffectSpecHandle GameplayEffectSpecHandle;

	// The actor that fired the projectile
	TWeakObjectPtr<AActor> InstigatorActor;
};
