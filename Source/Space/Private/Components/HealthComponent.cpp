// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AttributeSets/HealthSet.h"
#include "GameplayEffects/DamageEffect.h"

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
	if(ASC && GetOwner()->HasAuthority())
	{
		HealthSet = NewObject<UHealthSet>(GetOwner());
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
	if(!ASC)return;
	TSubclassOf<UGameplayEffect> DamageEffectClass = UDamageEffect::StaticClass();
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddInstigator(GetOwner(), GetOwner());

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, EffectContext);

	if(SpecHandle.IsValid())
	{
		FGameplayEffectSpec* spec = SpecHandle.Data.Get();
		if(spec)
		{
			spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Effects.Damage"), Damage);
		}

		ASC->ApplyGameplayEffectSpecToSelf(*spec);
	}
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



