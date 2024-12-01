// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Physics/GravityComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Components/SceneComponent.h"
#include "Components/MeshComponent.h"
#include "Utility/PriorityQueue.h"
#include "GravityVolume.h"

// Sets default values for this component's properties
UGravityComponent::UGravityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	GravityTag = FGameplayTag::RequestGameplayTag(FName("Physics.Gravity"));
}

void UGravityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	ApplyGravity();
}

void UGravityComponent::EnteredGravityVolume(UGravityVolume* volume)
{
	if(!volume)return;
	GravityZonesQueue.Push(volume, volume->GetVolumePriority());
}

void UGravityComponent::ExitedGravityVolume(UGravityVolume* volume)
{
	if(!volume)return;


	GravityZonesQueue.Remove(volume);
}

void UGravityComponent::RemoveGravityTag(const IAbilitySystemInterface& abilityOwner) const
{
	if (abilityOwner.GetAbilitySystemComponent()->HasMatchingGameplayTag(GravityTag))
	{
		abilityOwner.GetAbilitySystemComponent()->RemoveLooseGameplayTag(GravityTag);
	}
}

void UGravityComponent::AddGravityTag(const IAbilitySystemInterface& abilityOwner) const
{
	if (!abilityOwner.GetAbilitySystemComponent()->HasMatchingGameplayTag(GravityTag))
	{
		abilityOwner.GetAbilitySystemComponent()->AddLooseGameplayTag(GravityTag);
	}
}

void UGravityComponent::ApplyGravity() const
{
	if(IAbilitySystemInterface* abilityOwner = Cast<IAbilitySystemInterface>(GetOwner()))
	{
		//if no gravity
		if(GetGravityDirection().IsNearlyZero()){
			
			RemoveGravityTag(*abilityOwner);
			return;
		}
		//if has gravity add tag
		AddGravityTag(*abilityOwner);
	}
	
	USceneComponent* rootComp = GetOwner()->GetRootComponent();
	if(!rootComp ) return;

	UMeshComponent* shipMesh = Cast<UMeshComponent>(rootComp);
	if(!shipMesh)return;

	shipMesh->AddForce(GetGravityDirection() * shipMesh->GetMass(), FName("None"), true);
}


FVector UGravityComponent::GetGravityDirection() const
{
	if(GravityZonesQueue.IsEmpty() || GravityZonesQueue.Top() == nullptr )	return DefaultGravityDirection;

	return GravityZonesQueue.Top()->GetGravityDirection(GetOwner()->GetActorLocation());
}


