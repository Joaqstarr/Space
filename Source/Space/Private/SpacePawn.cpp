// Fill out your copyright notice in the Description page of Project Settings.


#include "SpacePawn.h"
#include "AbilitySystemComponent.h"
#include "Player/Abilities/ShipAbilitySystemComponent.h"
// Sets default values
ASpacePawn::ASpacePawn()
{
	SetReplicates(true);
	
	AbilitySystemComponent = CreateDefaultSubobject<UShipAbilitySystemComponent>( FName("AbilitySystemComponent"));
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AbilitySystemComponent->SetIsReplicated(true);
	bAlwaysRelevant = true;

}

UAbilitySystemComponent* ASpacePawn::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASpacePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}

	SetOwner(NewController);

	CurrentController = NewController;
	if (HasAuthority())
	{
		PosessedClient(NewController);
	}
}

void ASpacePawn::UnPossessed()
{
	Super::UnPossessed();

	if (HasAuthority())
	{
		UnPosessedClient(CurrentController);
	}
	CurrentController = nullptr;
}

// Called when the game starts or when spawned
void ASpacePawn::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);

	InitDefaultAttributes();
	GiveDefaultAbilities();
}

void ASpacePawn::PosessedClient_Implementation(AController* NewController)
{
	UE_LOG(LogTemp, Warning, TEXT("Running PosessedClient on %s"), HasAuthority() ? TEXT("Server") : TEXT("Client"));

	PosessedClientBP(NewController);
}

void ASpacePawn::UnPosessedClient_Implementation(AController* OldController)
{
	UE_LOG(LogTemp, Warning, TEXT("Running UnPosessedClient on %s"), HasAuthority() ? TEXT("Server") : TEXT("Client"));

	UnPosessedClientBP(OldController);
}

void ASpacePawn::InitDefaultAttributes() const
{
	if(!HasAuthority())return;
	if(!AbilitySystemComponent || !DefaultAttributeEffect)return;

	FGameplayEffectContextHandle effectHandle = AbilitySystemComponent->MakeEffectContext();

	effectHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle specHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, effectHandle);

	if(specHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*specHandle.Data.Get());
	}
}

void ASpacePawn::GiveDefaultAbilities()
{
	if(!HasAuthority()) return;
	check(AbilitySystemComponent);
	
	for(TSubclassOf<UGameplayAbility> abilityClass : DefaultAbilities)
	{
		const FGameplayAbilitySpec abilitySpec(abilityClass, 1);
		
		AbilitySystemComponent->GiveAbility(abilitySpec);
	}
}


