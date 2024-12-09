// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSets/HealthSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UHealthSet::UHealthSet() : USpaceAttributeSet()
{
	InitHealth(100);
}

void UHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
}

void UHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float localDamageDone = GetDamage();
		SetDamage(0);

		if(localDamageDone > 0)
		{
			const float NewHealth = GetHealth() - localDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0, GetMaxHealth()));	
		}
	}
	
}

void UHealthSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UHealthSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);

}


void UHealthSet::OnRep_Health(const FGameplayAttributeData& oldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthSet, Health, oldHealth);

}

void UHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& oldMaxHealth)
{

	GAMEPLAYATTRIBUTE_REPNOTIFY(UHealthSet, MaxHealth, oldMaxHealth);
	
}
