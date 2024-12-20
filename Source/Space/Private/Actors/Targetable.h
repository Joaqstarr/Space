// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Targetable.generated.h"

class UWidgetComponent;
class USphereComponent;
UCLASS()
class ATargetable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetable();

		
	UFUNCTION(BlueprintCallable, Category="Targeting")
	bool IsTargetable() const;

	UFUNCTION(BlueprintCallable, Category="Targeting")
	void SetTargetable(bool bTargetable);

	void EnteredRange();
	void ExitedRange();
	

	UFUNCTION(BlueprintCallable)
	FVector2D GetScreenPosition() const;
	void SetScreenPosition(const FVector2D& screenPosition, const bool bOnScreen);
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> CollisionSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> TargetIndicator;


private:
	UPROPERTY(EditAnywhere, Category="Targeting")
	bool bIsTargetable = true; // Default to targetable
	FVector2D ScreenPosition;
	bool bIsOnScreen = false;

};
