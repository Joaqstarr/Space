// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/ShootGunAbility.h"
#include "ShootGunAbility.h"
#include "Components/ObjectPooling/PoolManagerComponent.h"
#include "Utility/FactoryPayload.h"
#include "Projectiles/Projectile.h"
#include "AbilitySystemComponent.h"
#include "Actors/Targetable.h"

void UShootGunAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Validate pointers and get passed info

	if (!TriggerEventData || !ActorInfo)
	{
		return;
	}

	const UObject* Payload = TriggerEventData->OptionalObject;

	if (!Payload->Implements<UFactoryPayload>())
	{
		UE_LOG(LogTemp, Error, TEXT("ShootGunAbility: Invalid factory payload."));
		return;
	}

	AProjectile* Projectile = Cast<AProjectile>(IFactoryPayload::Execute_CreateInstance(Payload));
	
	// This could be because the pool ran out of objects or an mismatched factory type was received
	if (Projectile == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShootGunAbility: No projectile received from payload factory."));
		return;
	}

	if (ActorInfo->SkeletalMeshComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ShootGunAbility: No mesh set"));
		return;
	}

	TWeakObjectPtr<USkeletalMeshComponent> Mesh = ActorInfo->SkeletalMeshComponent;

	CommitAbility(Handle, ActorInfo, ActivationInfo);

	Projectile->SetActorLocation(Mesh->GetSocketLocation("Gun"));
	Projectile->SetActorRotation(Mesh->GetSocketRotation("Gun"));

	if (const ATargetable* LockOnTarget = Cast<ATargetable>(TriggerEventData->Target))
	{
		Projectile->SetTarget(LockOnTarget);
	}

	AActor* Avatar = ActorInfo->AvatarActor.Get();

	FInitializeProjectileParams projectileParams;
	projectileParams.Damage = Damage;
	projectileParams.InstigatorActor = Avatar;

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC != nullptr && OptionalGameplayEffectClass)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddInstigator(Avatar, Avatar);

		projectileParams.OptionalAdditionalEffect = ASC->MakeOutgoingSpec(OptionalGameplayEffectClass, 1.0f, EffectContext);
	}

	Projectile->InitializeProjectile(projectileParams);


	if (Projectile->Implements<UPoolableInterface>())
	{
		IPoolableInterface::Execute_Reset(Projectile);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	return;
}


