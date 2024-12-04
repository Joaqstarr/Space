// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffects/CooldownEffect.h"

#include "AttributeSets/HealthSet.h"

UCooldownEffect::UCooldownEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat durationMagnitudeSetByCallerFloat;
	durationMagnitudeSetByCallerFloat.DataTag = FGameplayTag::RequestGameplayTag("Effects.Cooldown");
	
	DurationMagnitude = FGameplayEffectModifierMagnitude(durationMagnitudeSetByCallerFloat);

}
