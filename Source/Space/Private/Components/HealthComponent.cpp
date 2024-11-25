// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UHealthComponent::SetMaxHealth(int newMax)
{
	MaxHealth = newMax;
	Health = FMath::Clamp(Health, 0, MaxHealth);
}

void UHealthComponent::UpdateHealth(int DeltaHealth)
{
	if(IsInvulnerable)return;
	if(DeltaHealth == 0) return;

	Health += DeltaHealth;
	Health = FMath::Clamp(Health, 0, MaxHealth);

	OnHealthChanged.Broadcast(Health, MaxHealth);

	if(Health <= 0)
	{
		OnHealthDepleted.Broadcast();
	}

	if(InvulnerabilityTime > 0)
	{
		IsInvulnerable = true;
		auto resetVulnerability = [&]() {IsInvulnerable = false;};
		GetOwner()->GetWorldTimerManager().SetTimer(IFramesTimer, resetVulnerability, InvulnerabilityTime, false);
	}
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	UpdateHealth(-Damage);
}



