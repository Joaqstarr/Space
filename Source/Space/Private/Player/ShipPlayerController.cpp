// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ShipPlayerController.h"
#include "AbilitySystemComponent.h"
#include "SpacePawn.h"

void AShipPlayerController::AcknowledgePossession(class APawn* p)
{
	Super::AcknowledgePossession(p);

	ASpacePawn* asSpacePawn = Cast<ASpacePawn>(p);

	if (asSpacePawn)
	{
		asSpacePawn->GetAbilitySystemComponent()->InitAbilityActorInfo(asSpacePawn, asSpacePawn);
	}
}

