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
	bool found = false;
	int hp = ASC->GetGameplayAttributeValue(UHealthSet::GetMaxHealthAttribute(), found);
	

	if (!found)
		UE_LOG(LogTemp, Error, TEXT("Get Health: Health Attribute Set not found"));

	return hp;
}

int UHealthComponent::GetMaxHealth() const
{
	bool found = false;
	int max = ASC->GetGameplayAttributeValue(UHealthSet::GetMaxHealthAttribute(), found);
	
	if (found)
		return max;
	
	UE_LOG(LogTemp, Error, TEXT("Get Max Health: Health Attribute Set not found"));

	return 1;
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

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ASC = GetAbilitySystemComponent();
	
	if(ASC && GetOwner()->HasAuthority())
	{
		HealthSet = NewObject<UHealthSet>(GetOwner());
		ASC->AddAttributeSetSubobject(HealthSet);
		
		ASC->GetGameplayAttributeValueChangeDelegate(UHealthSet::GetHealthAttribute()).AddUObject(this, &UHealthComponent::HealthAttributeChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UHealthSet::GetMaxHealthAttribute()).AddUObject(this, &UHealthComponent::MaxHealthAttributeChanged);

	}
	
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
	bool found;
	int max = ASC->GetGameplayAttributeValue(UHealthSet::GetMaxHealthAttribute(), found);

	MULTICASTHealthChange(data.NewValue, max);
	
}

void UHealthComponent::MaxHealthAttributeChanged(const FOnAttributeChangeData& data)
{
	bool found;
	int currentHp = ASC->GetGameplayAttributeValue(UHealthSet::GetHealthAttribute(), found);

	MULTICASTHealthChange(currentHp, data.NewValue);
}

void UHealthComponent::MULTICASTHealthChange_Implementation(float newHealth, float maxHealth)
{
	OnHealthChanged.Broadcast(newHealth, maxHealth);

	if(newHealth <= 0)
	{
		OnHealthDepleted.Broadcast();
	}
}



