// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpacePlayerController.generated.h"

class APlayerShip;
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
	virtual void ServerAcknowledgePossession_Implementation(class APawn* P) override;
	virtual void AcknowledgePossession(class APawn* p) override;

 
	UFUNCTION()
	void UnPossessPlayerShip();
	UFUNCTION()
	void PossessPlayerShip();
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintNativeEvent)
	void SetupDefaultWidget();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> DefaultWidgetClass;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UUserWidget> DefaultWidget;
private:
	UPROPERTY()
	TObjectPtr<APlayerShip> PlayerShipPawn;
	
	UFUNCTION(Server, Reliable)
	void UnPossessPlayerShip_Server();
	UFUNCTION(Server, Reliable)
	void PossessPlayerShip_Server();
};
