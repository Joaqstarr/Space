

#include "Player/SpacePlayerController.h"

#include "Blueprint/UserWidget.h"



void ASpacePlayerController::SetupDefaultWidget_Implementation()
{
	if (IsLocalPlayerController() && DefaultWidgetClass)
	{
		DefaultWidget = CreateWidget(this, DefaultWidgetClass, FName("ControllerWidget"));

		if (DefaultWidget)
		{
			DefaultWidget->AddToViewport(0);
		}
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller Widget creation failed. Is default class set?"));
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
