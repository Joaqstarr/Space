#include "Player/SpacePlayerController.h"
#include "PlayerShip.h"
#include "GameManagers/SpaceGameState.h"
#include "AbilitySystemComponent.h"
#include "SpacePawn.h"
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

void ASpacePlayerController::ServerAcknowledgePossession_Implementation(class APawn* P)
{
	Super::ServerAcknowledgePossession_Implementation(P);
	
	APlayerShip* asPlayerShip = Cast<APlayerShip>(P);
		
	if (asPlayerShip)
	{
		PlayerShipPawn = asPlayerShip;
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





void ASpacePlayerController::UnPossessPlayerShip()
{
	UnPossessPlayerShip_Server();
}

void ASpacePlayerController::UnPossessPlayerShip_Server_Implementation()
{
	UnPossess();
}

void ASpacePlayerController::PossessPlayerShip()
{
	PossessPlayerShip_Server_Implementation();
}

void ASpacePlayerController::PossessPlayerShip_Server_Implementation()
{
	Possess(PlayerShipPawn);
}

void ASpacePlayerController::BeginPlay()
{
	Super::BeginPlay();

	ASpaceGameState* gameState = Cast<ASpaceGameState>(GetWorld()->GetGameState());
	if (gameState)
	{
		if (HasAuthority())
		{
			gameState->OnSwitchToMapStateEvent.AddDynamic(this, &ASpacePlayerController::UnPossessPlayerShip);
			gameState->OnSwitchToCombatZoneStateEvent.AddDynamic(this, &ASpacePlayerController::PossessPlayerShip);
		}
	}
	
	if (IsLocalPlayerController())
	{
		SetupDefaultWidget();
	}
}

