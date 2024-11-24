// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TargetableComponent.generated.h"


class USphereComponent;
class UWidgetComponent;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTargetableComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetableComponent(const FObjectInitializer& OI);
	
	UFUNCTION(BlueprintCallable, Category="Targeting")
	bool IsTargetable() const;

	UFUNCTION(BlueprintCallable, Category="Targeting")
	void SetTargetable(bool bTargetable);

	UFUNCTION(BlueprintCallable)
	FVector2D GetScreenPosition() const;
	void SetScreenPosition(const FVector2D& screenPosition, const bool bOnScreen);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> CollisionSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> TargetIndicator;
protected:
	virtual void BeginPlay() override;



private:
	UPROPERTY(EditAnywhere, Category="Targeting")
	bool bIsTargetable = true; // Default to targetable
	FVector2D ScreenPosition;
	bool bIsOnScreen = false;
};
