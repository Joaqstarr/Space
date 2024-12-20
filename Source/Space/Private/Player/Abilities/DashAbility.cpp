// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/DashAbility.h"

#include "AbilitySystemComponent.h"
#include "Actors/Targetable.h"
#include "AttributeSets/DashSet.h"
#include "Player/PlayerShip.h"
#include "GameplayAbilities/Public/Abilities/GameplayAbilityTargetTypes.h"

UDashAbility::UDashAbility()
{
	FGameplayTag dashTag {FGameplayTag::RequestGameplayTag(FName("Ship.Action.Dash"))};
	AbilityTags.AddTag(dashTag);

	ActivationOwnedTags.AddTag(dashTag);
	ActivationBlockedTags.AddTag(dashTag);

	FAbilityTriggerData triggerEvent;
	triggerEvent.TriggerTag = dashTag;
	AbilityTriggers.Add(triggerEvent);

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	
}

void UDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (TriggerEventData->TargetData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("DashAbility: No Target Data"));
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}

	//retrieving direction vector from target hit result impact point
	const FGameplayAbilityTargetData_SingleTargetHit* payload = static_cast<FGameplayAbilityTargetData_SingleTargetHit*>(TriggerEventData->TargetData.Data[0].Get());
	if (!payload)
	{
		UE_LOG(LogTemp, Error, TEXT("DashAbility: Target Data Cast Failed"));

		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
		return;
	}
	Dash(payload->HitResult.ImpactPoint);

	CommitAbility(Handle, ActorInfo, ActivationInfo, nullptr);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}


void UDashAbility::Dash(const FVector& inputDir)
{
	FVector swizzled(inputDir.Y, inputDir.X, inputDir.Z);
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, swizzled.ToString());
	UPrimitiveComponent* root = Cast<UPrimitiveComponent>(CurrentActorInfo->AvatarActor->GetRootComponent());
	
	if(root)
	{
		FVector transformedDir { root->GetComponentTransform().TransformVector(swizzled).GetSafeNormal()};

		float strength = 0;
		if(const UDashSet* dashSet = CurrentActorInfo->AbilitySystemComponent->GetSet<UDashSet>())
		{
			strength = dashSet->GetDashStrength();
		}

		//UpdateDirectionAndStrengthForTarget(transformedDir, strength);
		
		//root->SetWorldLocation(root->GetComponentLocation() + transformedDir * strength, true, nullptr, ETeleportType::TeleportPhysics);
		root->AddImpulse(transformedDir * strength, NAME_None, true);
	}
}

void UDashAbility::UpdateDirectionAndStrengthForTarget(FVector& dir, float& strength)
{
	APlayerShip* asPlayer = Cast<APlayerShip>(CurrentActorInfo->AvatarActor);

	if(!asPlayer)return;

	ATargetable* target = asPlayer->GetCurrentTarget();
	if(!target)return;
	
	float distToTarget = FVector::Distance(target->GetActorLocation(), asPlayer->GetActorLocation());
		
	if(distToTarget > strength * 1.4f)return;
	
	FVector dirToTarget {(target->GetActorLocation() - asPlayer->GetActorLocation()).GetSafeNormal()};
	float dot = dir.Dot(dirToTarget);

	//if not dashing towards target, return
	if(dot <= 0.5f)return;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Dot: %f"), dot));


	

	
	//FVector aheadPos = ULookAtComponent::LookAheadByVelocity(target->GetActorLocation(), target->GetRootComponent()->GetComponentVelocity(), asPlayer->GetActorLocation(), strength);
	dir = dirToTarget; //(aheadPos - asPlayer->GetActorLocation()).GetSafeNormal();
	strength = distToTarget -100;
	
}

