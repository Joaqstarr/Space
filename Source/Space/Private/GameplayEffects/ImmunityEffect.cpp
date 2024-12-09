// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffects/ImmunityEffect.h"

#include "AttributeSets/HealthSet.h"
#include "GameplayEffectComponents/ImmunityGameplayEffectComponent.h"

UImmunityEffect::UImmunityEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	UImmunityGameplayEffectComponent* immunityGameplayEffectComponent = CreateDefaultSubobject<UImmunityGameplayEffectComponent>("ImmunityGameplayEffectComponent");

	FGameplayEffectQuery immunityQuery;
	immunityQuery.ModifyingAttribute = UHealthSet::GetHealthAttribute();
	
	immunityGameplayEffectComponent->ImmunityQueries.Add(immunityQuery);
	
	GEComponents.Add(immunityGameplayEffectComponent);
}
