// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagers/States/CombatZoneGameModeState.h"
#include "Kismet/GameplayStatics.h"
#include "GameManagers/LevelStreaming/LevelStreamingSubsystem.h"

void UCombatZoneGameModeState::OnEnterState_Implementation()
{
	if (ULevelStreamingSubsystem* levelStreamingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<ULevelStreamingSubsystem>())
	{
		levelStreamingSubsystem->StreamLevel(FName("DeepSpaceMap"), true);
	}
}

void UCombatZoneGameModeState::OnExitState_Implementation()
{
	if (ULevelStreamingSubsystem* levelStreamingSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<ULevelStreamingSubsystem>())
	{
		levelStreamingSubsystem->UnloadLevel(FName("DeepSpaceMap"));
	}
}
