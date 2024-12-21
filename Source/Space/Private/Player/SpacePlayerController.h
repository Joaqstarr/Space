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
public:
	virtual void AcknowledgePossession(class APawn* p) override;
	UFUNCTION(BlueprintCallable)
	virtual void EnterPlayerController();

	UFUNCTION(BlueprintCallable)
	virtual void ExitPlayerController();
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintNativeEvent)
	void SetupDefaultWidget();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> DefaultWidgetClass;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UUserWidget> DefaultWidget;
};
