// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ShipParts/TurretComponent.h"

#include "LookAtComponent.h"

// Sets default values for this component's properties
UTurretComponent::UTurretComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TurretBaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("TurretBaseMesh"));
	TurretHeadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("TurretHeadMesh"));
	LookAtComponent = CreateDefaultSubobject<ULookAtComponent>(FName("TurretLookAt"));
	
	TurretBaseMeshComponent->SetupAttachment(this);
	LookAtComponent->SetupAttachment(TurretBaseMeshComponent);
	TurretHeadMeshComponent->SetupAttachment(LookAtComponent);
}


// Called when the game starts
void UTurretComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTurretComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

