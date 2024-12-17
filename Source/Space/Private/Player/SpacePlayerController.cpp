// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SpacePlayerController.h"

#include "AbilitySystemComponent.h"
#include "SpacePawn.h"

void ASpacePlayerController::AcknowledgePossession(class APawn* p)
{
	Super::AcknowledgePossession(p);

	ASpacePawn* asSpacePawn = Cast<ASpacePawn>(p);

	if (asSpacePawn)
	{
		asSpacePawn->GetAbilitySystemComponent()->InitAbilityActorInfo(asSpacePawn, asSpacePawn);
	}
}
