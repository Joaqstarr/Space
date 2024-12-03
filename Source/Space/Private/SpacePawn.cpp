// Fill out your copyright notice in the Description page of Project Settings.


#include "SpacePawn.h"
#include "AbilitySystemComponent.h"
#include "Player/Abilities/ShipAbilitySystemComponent.h"
// Sets default values
ASpacePawn::ASpacePawn()
{
	AbilitySystemComponent = CreateDefaultSubobject<UShipAbilitySystemComponent>( FName("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

}

UAbilitySystemComponent* ASpacePawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void ASpacePawn::BeginPlay()
{
	Super::BeginPlay();
	InitDefaultAttributes();
	GiveDefaultAbilities();

}

void ASpacePawn::InitDefaultAttributes() const
{
	if(!AbilitySystemComponent || !DefaultAttributeEffect)return;

	FGameplayEffectContextHandle effectHandle = AbilitySystemComponent->MakeEffectContext();

	effectHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle specHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, effectHandle);

	if(specHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());
	}
}

void ASpacePawn::GiveDefaultAbilities()
{
	check(AbilitySystemComponent);
	if(!HasAuthority()) return;
	
	for(TSubclassOf<UGameplayAbility> abilityClass : DefaultAbilities)
	{
		const FGameplayAbilitySpec abilitySpec(abilityClass, 1);
		
		AbilitySystemComponent->GiveAbility(abilitySpec);
	}
}


