// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AttributeSets/HealthSet.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	IAbilitySystemInterface* asASC = Cast<IAbilitySystemInterface>(GetOwner());
	if(asASC)
	{
		ASC = asASC->GetAbilitySystemComponent();
	}
	if(ASC)
	{
		HealthSet = NewObject<UHealthSet>();
		ASC->AddAttributeSetSubobject(HealthSet);
		
		ASC->GetGameplayAttributeValueChangeDelegate(UHealthSet::GetHealthAttribute()).AddUObject(this, &UHealthComponent::HealthAttributeChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UHealthSet::GetMaxHealthAttribute()).AddUObject(this, &UHealthComponent::MaxHealthAttributeChanged);

		bool found = false;
		MaxHealth = ASC->GetGameplayAttributeValue(UHealthSet::GetMaxHealthAttribute(), found);
	}
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	//TODO apply damage effect
}

void UHealthComponent::HealthAttributeChanged(const FOnAttributeChangeData& data)
{
	OnHealthChanged.Broadcast(data.NewValue, MaxHealth);
	Health = data.NewValue;
	if(data.NewValue <= 0)
	{
		OnHealthDepleted.Broadcast();
	}
}

void UHealthComponent::MaxHealthAttributeChanged(const FOnAttributeChangeData& data)
{
	MaxHealth = data.NewValue;
}



