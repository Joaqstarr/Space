// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceGamemode.h"

#include "Kismet/GameplayStatics.h"
#include "Map/MapTransformComponent.h"
#include "Map/OverworldMap.h"
#include "Map/Player/MapPlayer.h"


ASpaceGamemode::ASpaceGamemode()
{
	MapPlayerClass = AMapPlayer::StaticClass();
}

void ASpaceGamemode::BeginPlay()
{
	Super::BeginPlay();
	GenerateOverworldMap(FWorldGenerationParams());
	
	OverworldMapActor = Cast<AOverworldMap>(UGameplayStatics::GetActorOfClass(GetWorld(), AOverworldMap::StaticClass()));

	//GetWorld()->GetFirstPlayerController()->Possess(MapPlayerPawn);
}

FOverworldMapData& ASpaceGamemode::GetMapData()
{
	return OverworldMap;
}

AOverworldMap* ASpaceGamemode::GetOverworldMap()
{
	return OverworldMapActor;
}

FVector GenerateRandomNormalizedVector()
{
	// Generate a random angle in radians
	float RandomAngle = FMath::RandRange(0.0f, 2.0f * PI);

	// Compute the x and y components using trigonometric functions
	float X = FMath::Cos(RandomAngle);
	float Y = FMath::Sin(RandomAngle);

	// Return the normalized vector (it will already be normalized due to trigonometry)
	return FVector(X, Y, 0);
}

void ASpaceGamemode::GenerateOverworldMap(const FWorldGenerationParams& params)
{
	float minDistance = 0.1f;
	float maxDistance = 0.5f;
	int numPlanets = 5;
	//basic map gen code
	for (int i = 0; i < numPlanets; i++)
	{
		float distanceInterp = static_cast<float>(i) / numPlanets;
		float distanceFromCenter = FMath::Lerp(minDistance, maxDistance, distanceInterp);

		FVector randomVector = GenerateRandomNormalizedVector();

		randomVector *= distanceFromCenter;

		//center random vector around star(located at 0.5, 0.5)
		randomVector += FVector(0.5f, 0.5f, 0.0f);


		FPlanetData planet;
		planet.Name = FName(TEXT("Planet ") + FString::FromInt(i + 1));
		planet.Location = randomVector;

		FBaseData baseData;
		baseData.Name = FName(TEXT("Base ") + FString::FromInt(i + 1));

		planet.Bases.Add(baseData);
		OverworldMap.Planets.Add(planet);
	}
}



