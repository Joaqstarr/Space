// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSets/DashSet.h"

#include "Net/UnrealNetwork.h"

void UDashSet::OnRep_DashStrength(const FGameplayAttributeData& oldDashStrength)
{

	//GAMEPLAYATTRIBUTE_REPNOTIFY(UDashSet, DashStrength, oldDashStrength);
	static FProperty* ThisProperty = FindFieldChecked<FProperty>(UDashSet::StaticClass(), GET_MEMBER_NAME_CHECKED(UDashSet, DashStrength));
	GetOwningAbilitySystemComponentChecked()->SetBaseAttributeValueFromReplication(FGameplayAttribute(ThisProperty), DashStrength, oldDashStrength);
}

UDashSet::UDashSet()
{
	InitDashStrength(100);
}

void UDashSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDashSet, DashStrength, COND_None, REPNOTIFY_Always);
	
}
