// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagers/States/MapGameModeState.h"

#include "GameManagers/GameMode/SpaceGamemode.h"
#include "Map/MapTransformComponent.h"
#include "Map/Player/MapPlayer.h"

UMapGameModeState::UMapGameModeState()
{
	MapPlayerClass = AMapPlayer::StaticClass();
}

void UMapGameModeState::OnEnterState_Implementation()
{
	Super::OnEnterState_Implementation();

	if (OverworldMapActor == nullptr)
	{
		if (ASpaceGamemode* gameMode = Cast<ASpaceGamemode>(GetWorld()->GetAuthGameMode()))
		{
			OverworldMapActor = gameMode->GetOverworldMap();
		}
	}
	if (HasAuthority())
	{
		if (MapPlayerPawn == nullptr)
		{
			FActorSpawnParameters spawnParams;
			MapPlayerPawn = GetWorld()->SpawnActor<AMapPlayer>(MapPlayerClass, spawnParams);

			MapPlayerPawn->GetMapTransformComponent()->SetAssociatedMap(OverworldMapActor);
		}

		if (MapPlayerPawn != nullptr)
		{
			if (GetWorld()->GetFirstPlayerController())
			{
				GetWorld()->GetFirstPlayerController()->Possess(MapPlayerPawn);
			}
		}
	}

	
}

void UMapGameModeState::OnExitState_Implementation()
{
	Super::OnExitState_Implementation();
}
