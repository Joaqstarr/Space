// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/States/CombatZoneGameModeState.h"

#include "Kismet/GameplayStatics.h"

void UCombatZoneGameModeState::OnEnterState_Implementation()
{
	UGameplayStatics::LoadStreamLevel(GetWorld(), FName("DeepSpaceMap"), true, true, FLatentActionInfo());
}

void UCombatZoneGameModeState::OnExitState_Implementation()
{
	UGameplayStatics::UnloadStreamLevel(GetWorld(), FName("DeepSpaceMap"), FLatentActionInfo(), true);
}
