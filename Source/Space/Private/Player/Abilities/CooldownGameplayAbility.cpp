// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Abilities/CooldownGameplayAbility.h"

#include "GameplayEffects/CooldownEffect.h"

UCooldownGameplayAbility::UCooldownGameplayAbility()
{
	CooldownGameplayEffectClass = UCooldownEffect::StaticClass();
}

const FGameplayTagContainer* UCooldownGameplayAbility::GetCooldownTags() const
{
	FGameplayTagContainer* mutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	mutableTags->Reset(); // MutableTags writes to the TempCooldownTags on the CDO so clear it in case the ability cooldown tags change (moved to a different slot)
	const FGameplayTagContainer* parentTags = Super::GetCooldownTags();
	if (parentTags)
	{
		mutableTags->AppendTags(*parentTags);
	}
	mutableTags->AppendTags(CooldownTags);
	return mutableTags;
}

void UCooldownGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle handle,
	const FGameplayAbilityActorInfo* actorInfo, const FGameplayAbilityActivationInfo activationInfo) const
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Cooldown applied: %f"), CooldownDuration.GetValueAtLevel(GetAbilityLevel())));
	if (UGameplayEffect* cooldownGE = GetCooldownGameplayEffect())
	{
		FGameplayEffectSpecHandle specHandle = MakeOutgoingGameplayEffectSpec(cooldownGE->GetClass(), GetAbilityLevel());
		specHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		specHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Effects.Cooldown")), CooldownDuration.GetValueAtLevel(GetAbilityLevel()));
		ApplyGameplayEffectSpecToOwner(handle, actorInfo, activationInfo, specHandle);
	}
}
