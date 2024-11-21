// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ShipParts/TurretComponent.h"

#include "LookAtComponent.h"

// Sets default values for this component's properties
UTurretComponent::UTurretComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TurretBaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Turret Base Mesh");
	TurretHeadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Turret Head Mesh");
	LookAtComponent = CreateDefaultSubobject<ULookAtComponent>("Turret Look At");
	
	TurretBaseMeshComponent->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	LookAtComponent->AttachToComponent(TurretBaseMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	TurretHeadMeshComponent->AttachToComponent(LookAtComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
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

