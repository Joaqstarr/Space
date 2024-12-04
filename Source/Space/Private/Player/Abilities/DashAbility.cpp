// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/DashAbility.h"

#include "AbilitySystemComponent.h"
#include "Actors/Targetable.h"
#include "AttributeSets/DashSet.h"
#include "Player/PlayerShip.h"
#include "Utility/VectorPayload.h"

UDashAbility::UDashAbility()
{
	FGameplayTag dashTag {FGameplayTag::RequestGameplayTag(FName("Ship.Action.Dash"))};
	AbilityTags.AddTag(dashTag);

	ActivationOwnedTags.AddTag(dashTag);
	ActivationBlockedTags.AddTag(dashTag);

	FAbilityTriggerData triggerEvent;
	triggerEvent.TriggerTag = dashTag;
	AbilityTriggers.Add(triggerEvent);

	
}

void UDashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const UVectorPayload* payload = Cast<UVectorPayload>(TriggerEventData->OptionalObject);
	Dash(payload->VectorData);
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

		UpdateDirectionAndStrengthForTarget(transformedDir, strength);
		
		root->AddImpulse(transformedDir * strength, NAME_None, true);
	}
}

void UDashAbility::UpdateDirectionAndStrengthForTarget(FVector& dir, float& strength)
{
	APlayerShip* asPlayer = Cast<APlayerShip>(CurrentActorInfo->AvatarActor);

	if(!asPlayer)return;

	ATargetable* target = asPlayer->GetCurrentTarget();
//todo dist check
	if(!target)return;
	FVector dirToTarget {(target->GetActorLocation() - asPlayer->GetActorLocation()).GetSafeNormal()};
	float dot = dir.Dot(dirToTarget);

	//if not dashing towards target, return
	if(dot <= 0.5f)return;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Dot: %f"), dot));

	dir = dirToTarget;

	strength = FVector::Distance(target->GetActorLocation(), asPlayer->GetActorLocation())/0.7f;
	
	asPlayer->SetAutoLookAtTarget(target->GetActorLocation());

	//GetWorld()->GetWorldSettings()->SetTimeDilation(0.2f);
}

