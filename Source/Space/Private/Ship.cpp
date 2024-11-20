// Fill out your copyright notice in the Description page of Project Settings.


#include "Ship.h"
#include "Components/StaticMeshComponent.h"
#include "Components/Physics/GravityComponent.h"

// Sets default values
AShip::AShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("StaticShipMesh")));
	ShipMesh->SetSimulatePhysics(true);
	ShipMesh->SetEnableGravity(false);

	SetRootComponent(ShipMesh);
	
	GravityComponent = CreateDefaultSubobject<UGravityComponent>("Gravity Component");
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AShip::AddRoll(float rollAmount)
{
}

void AShip::AddPitch(float rollAmount)
{
	
}

void AShip::AddYaw(float yawAmount)
{
}

void AShip::AddThrust(float forwardThrust, float sidewaysThrust)
{
}

