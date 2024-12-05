// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Weapons/Sword.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
}

void ASword::EnableSword_Implementation()
{
}

void ASword::DisableSword_Implementation()
{
}

// Called when the game starts or when spawned
void ASword::BeginPlay()
{
	Super::BeginPlay();
	
}


