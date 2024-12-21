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

int UHealthComponent::GetHealth() const
{
	return Health;
}

int UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

UAbilitySystemComponent* UHealthComponent::GetAbilitySystemComponent() const
{

	IAbilitySystemInterface* asASC = Cast<IAbilitySystemInterface>(GetOwner());
	if(asASC)
	{
		return  asASC->GetAbilitySystemComponent();
	}
	return nullptr;
}

void UHealthComponent::InitializeHealthValues()
{
	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UHealthSet::GetHealthAttribute()).AddUObject(this, &UHealthComponent::HealthAttributeChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UHealthSet::GetMaxHealthAttribute()).AddUObject(this, &UHealthComponent::MaxHealthAttributeChanged);
		bool found = false;
		Health = ASC->GetGameplayAttributeValue(UHealthSet::GetMaxHealthAttribute(), found);
		MaxHealth = ASC->GetGameplayAttributeValue(UHealthSet::GetMaxHealthAttribute(), found);
	}
}

void UHealthComponent::CreateHealthAttributeSet()
{
	if(ASC && GetOwner()->HasAuthority())
	{
		HealthSet = NewObject<UHealthSet>(GetOwner());
		ASC->AddAttributeSetSubobject(HealthSet);
		
		
	}
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ASC = GetAbilitySystemComponent();
	
	CreateHealthAttributeSet();
	InitializeHealthValues();
	
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
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
	Health = data.NewValue;
	OnHealthChanged.Broadcast(Health, data.OldValue,MaxHealth);

	if(Health <= 0)
	{
		OnHealthDepleted.Broadcast();
	}
}

void UHealthComponent::MaxHealthAttributeChanged(const FOnAttributeChangeData& data)
{
	MaxHealth = data.NewValue;
}





