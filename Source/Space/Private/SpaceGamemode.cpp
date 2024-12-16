// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceGamemode.h"
#include "PhysicsEngine/PhysicsSettings.h"

void ASpaceGamemode::StartPlay()
{
	Super::StartPlay();
	
	UPhysicsSettings* physicsSettings = UPhysicsSettings::Get();
	if (physicsSettings)
	{
		physicsSettings->bSubstepping = true;
		physicsSettings->MaxSubstepDeltaTime = 1.0f / 60.0f; // Match desired frame rate
		physicsSettings->MaxSubsteps = 5;

		UE_LOG(LogTemp, Log, TEXT("Physics Substepping Configured: MaxSubstepDeltaTime = %f, MaxSubsteps = %d"),
			physicsSettings->MaxSubstepDeltaTime, physicsSettings->MaxSubsteps);
	}
	
}
