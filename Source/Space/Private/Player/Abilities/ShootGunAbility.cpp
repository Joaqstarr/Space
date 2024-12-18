// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/ShootGunAbility.h"
#include "ShootGunAbility.h"
#include "Components/ObjectPooling/PoolManagerComponent.h"
#include "Utility/FactoryPayload.h"
#include "Utility/TransformPayload.h"
#include "Projectiles/Projectile.h"
#include "AbilitySystemComponent.h"
#include "Actors/Targetable.h"

UShootGunAbility::UShootGunAbility()
{
	FGameplayTag AbilityTag{ FGameplayTag::RequestGameplayTag(FName("Ship.Action.ShootGun")) };
	AbilityTags.AddTag(AbilityTag);

	FAbilityTriggerData TriggerEvent;
	TriggerEvent.TriggerTag = AbilityTag;
	AbilityTriggers.Add(TriggerEvent);

}

void UShootGunAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Validate pointers and get passed info
	if (!TriggerEventData || !ActorInfo)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	const UObject* ProjectileFactoryPayload = TriggerEventData->OptionalObject;

	if (!ProjectileFactoryPayload || !ProjectileFactoryPayload->Implements<UFactoryPayload>())
	{
		UE_LOG(LogTemp, Error, TEXT("ShootGunAbility: Invalid factory payload."));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	const UTransformPayload* SpawnTransform = Cast<UTransformPayload>(TriggerEventData->OptionalObject2);
	if (!SpawnTransform)
	{
		UE_LOG(LogTemp, Error, TEXT("ShootGunAbility: No spawn transform specified."));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	AProjectile* Projectile = Cast<AProjectile>(IFactoryPayload::Execute_CreateInstance(ProjectileFactoryPayload));
	
	// This could be because the pool ran out of objects or an mismatched factory type was received
	if (Projectile == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShootGunAbility: No projectile received from payload factory."));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		Projectile->Destroy();
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	Projectile->SetActorLocation(SpawnTransform->TransformData.GetLocation());
	Projectile->SetActorRotation(SpawnTransform->TransformData.GetRotation());

	if (const ATargetable* LockOnTarget = Cast<ATargetable>(TriggerEventData->Target))
	{
		Projectile->SetTarget(LockOnTarget);
	}

	AActor* Avatar = ActorInfo->AvatarActor.Get();

	FInitializeProjectileParams projectileParams;
	projectileParams.Damage = Damage;
	projectileParams.InstigatorActor = Avatar;

	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	if (ASC != nullptr && OptionalProjectileEffect)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddInstigator(Avatar, Avatar);

		projectileParams.OptionalAdditionalEffect = ASC->MakeOutgoingSpec(OptionalProjectileEffect, 1.0f, EffectContext);
	}

	Projectile->InitializeProjectile(projectileParams);


	if (Projectile->Implements<UPoolableInterface>())
	{
		IPoolableInterface::Execute_Reset(Projectile);
	}

	if (ASC != nullptr && ShootEffect)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();

		EffectContext.AddOrigin(SpawnTransform->TransformData.GetLocation());
		EffectContext.AddSourceObject(SpawnTransform);

		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(ShootEffect, 1.0f, EffectContext);
		ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	return;
}


