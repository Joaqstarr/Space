// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/DashAbility.h"

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
		FVector transformedDir { root->GetComponentTransform().TransformVector(swizzled)};
		
		root->AddImpulse(transformedDir * 10000, NAME_None, true);
	}
}

