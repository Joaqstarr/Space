// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/UIManager.h"

// Sets default values for this component's properties
UUIManager::UUIManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUIManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UUIManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
/*
void UGamemodeBaseState::SetupWidget_Implementation()
{
	if (!HasAuthority())
	{
		if (DefaultWidgetClass)
		{
			DefaultWidget = CreateWidget(this, DefaultWidgetClass, FName("ControllerWidget"));

			if (DefaultWidget)
			{
				DefaultWidget->AddToViewport(0);
			}
		}else
		{
			UE_LOG(LogTemp, Warning, TEXT("Controller Widget creation failed. Is default class set?"));
		}
	}

	
}*/

