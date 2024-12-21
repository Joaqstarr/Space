// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelStreamingSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ULevelStreamingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "LevelStreaming")
	void StreamLevel(const FName& levelName, bool bShouldBlockOnLoad = false);
	UFUNCTION( BlueprintCallable, Category = "Level Streaming")
	void UnloadLevel(const FName& levelName);

private:
	// Keep track of streamed levels
	TArray<FName> StreamedLevels;
};
