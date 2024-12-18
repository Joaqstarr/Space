// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpacePlayerController.generated.h"

class UUserWidget;
class APawn;
/**
 * 
 */
UCLASS()
class ASpacePlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintNativeEvent)
	void SetupDefaultWidget();
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> DefaultWidgetClass;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UUserWidget> DefaultWidget;
};
