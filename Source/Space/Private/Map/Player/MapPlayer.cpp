// Fill out your copyright notice in the Description page of Project Settings.


#include "Map/Player/MapPlayer.h"

#include "Map/MapTransformComponent.h"

// Sets default values
AMapPlayer::AMapPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MapTransformComponent = CreateDefaultSubobject<UMapTransformComponent>(FName("MapTransformComponent"));
}

// Called when the game starts or when spawned
void AMapPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMapPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UMapTransformComponent* AMapPlayer::GetMapTransformComponent()
{
	return MapTransformComponent;
}

void AMapPlayer::AddMapMovementInputServer_Implementation(FVector WorldDirection, float ScaleValue)
{
	AddMapMovementInput(WorldDirection, ScaleValue);

}

void AMapPlayer::TryAddMovementInput(FVector WorldDirection, float ScaleValue)
{
	AddMapMovementInputServer(WorldDirection, ScaleValue);
	AddMapMovementInput(WorldDirection, ScaleValue);

}

void AMapPlayer::AddMapMovementInput(FVector WorldDirection, float ScaleValue)
{
	FVector newPos = MapTransformComponent->MapPosition + WorldDirection * ScaleValue * GetWorld()->GetDeltaSeconds();
	
	//clamp axis to 0-1 so it never leaves map
	newPos.X = FMath::Clamp(newPos.X, 0, 1);
	newPos.Y = FMath::Clamp(newPos.Y, 0, 1);
	newPos.Z = FMath::Clamp(newPos.Z, 0, 1);
	
	MapTransformComponent->SetMapPosition(newPos);
}

