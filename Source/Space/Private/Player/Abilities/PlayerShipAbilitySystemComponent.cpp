// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerShipAbilitySystemComponent.h"


UPlayerShipAbilitySystemComponent::UPlayerShipAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);
}


// Called when the game starts
void UPlayerShipAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerShipAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

