// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffects/DamageEffect.h"

#include "AttributeSets/HealthSet.h"

UDamageEffect::UDamageEffect() : UGameplayEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo modifierInfo;
	modifierInfo.Attribute = UHealthSet::GetDamageAttribute();

	FSetByCallerFloat setByCallerValue;
	setByCallerValue.DataTag = FGameplayTag::RequestGameplayTag(FName("Effects.Damage"));
	modifierInfo.ModifierOp = EGameplayModOp::Additive;
	modifierInfo.ModifierMagnitude = FGameplayEffectModifierMagnitude(setByCallerValue);
	
	Modifiers.Add(modifierInfo);
}
