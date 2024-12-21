// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManagers/LevelStreaming/LevelStreamingSubsystem.h"

#include "GameManagers/SpaceGameState.h"
#include "Kismet/GameplayStatics.h"



void ULevelStreamingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void ULevelStreamingSubsystem::Deinitialize()
{
	Super::Deinitialize();
}




void ULevelStreamingSubsystem::StreamLevel(const FName& levelName, bool bShouldBlockOnLoad)
{
	if (!StreamedLevels.Contains(levelName))
	{
		UGameplayStatics::LoadStreamLevel(this, levelName, true, bShouldBlockOnLoad, FLatentActionInfo());
		StreamedLevels.Add(levelName);
		UE_LOG(LogTemp, Log, TEXT("Streaming Level: %s"), *levelName.ToString());
	}
}


void ULevelStreamingSubsystem::UnloadLevel(const FName& levelName)
{
	if (StreamedLevels.Contains(levelName))
	{
		UGameplayStatics::UnloadStreamLevel(this, levelName, FLatentActionInfo(), true);
		StreamedLevels.Remove(levelName);
		UE_LOG(LogTemp, Log, TEXT("Unloading Level: %s"), *levelName.ToString());
	}
}
