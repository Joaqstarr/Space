// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ObjectPooling/PoolableInterface.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UTargetableComponent;
class USphereComponent;
class UProjectileMovementComponent;

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

	void SetTarget(UTargetableComponent* target);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile Stats")
	float LifeTime = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Projectile Stats")
	float MaxSpeed = 600;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereHitbox;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	
private:
	UPROPERTY()
	FTimerHandle DespawnTimerHandle;

	UFUNCTION()
	void OnProjectileMCActivated( UActorComponent* Component, bool bReset);

	void DisableHomingOffAngle(float minDot);
};
