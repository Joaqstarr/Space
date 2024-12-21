

#include "Player/SpacePlayerController.h"

#include "AbilitySystemComponent.h"
#include "SpacePawn.h"
#include "Blueprint/UserWidget.h"



void ASpacePlayerController::SetupDefaultWidget_Implementation()
{
	if (IsLocalPlayerController() && DefaultWidgetClass)
	{
		DefaultWidget = CreateWidget(this, DefaultWidgetClass, FName("ControllerWidget"));

		EnterPlayerController();

	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller Widget creation failed. Is default class set?"));
	}
	
}

void ASpacePlayerController::AcknowledgePossession(class APawn* p)
{
	Super::AcknowledgePossession(p);

	ASpacePawn* asSpacePawn = Cast<ASpacePawn>(p);

	if (asSpacePawn)
	{
		asSpacePawn->GetAbilitySystemComponent()->InitAbilityActorInfo(asSpacePawn, asSpacePawn);
	}
}

void ASpacePlayerController::EnterPlayerController()
{
	if (DefaultWidget)
	{
		DefaultWidget->AddToViewport(0);
	}
}

void ASpacePlayerController::ExitPlayerController()
{
	if (IsLocalPlayerController() && DefaultWidget)
	{
		DefaultWidget->RemoveFromParent();
	}
}

void ASpacePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		SetupDefaultWidget();
	}
}
