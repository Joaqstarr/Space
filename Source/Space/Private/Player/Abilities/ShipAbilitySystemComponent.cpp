// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipAbilitySystemComponent.h"


UShipAbilitySystemComponent::UShipAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);


}


// Called when the game starts
void UShipAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UShipAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

