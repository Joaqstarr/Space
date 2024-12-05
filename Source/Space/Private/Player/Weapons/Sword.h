// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

class UStaticMeshComponent;

UCLASS()
class ASword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASword();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnableSword();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisableSword();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> SwordMesh;
};
