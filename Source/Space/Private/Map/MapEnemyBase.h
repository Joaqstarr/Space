// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapObject.h"
#include "GameFramework/Actor.h"
#include "MapEnemyBase.generated.h"

class AMapReinforcementUnit;
class UMapTransformComponent;
class UTokenConsumer;

UCLASS()
class AMapEnemyBase : public AActor, public IMapObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapEnemyBase();
	virtual void Tick(float DeltaTime) override;

	UTokenConsumer* GetTokenConsumer();
	
	//null check result
	virtual UMapTransformComponent* GetMapTransformComponent() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UTokenConsumer> TokenConsumer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UMapTransformComponent> MapTransform;

	UPROPERTY()
	TObjectPtr<USceneComponent> RootSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AMapReinforcementUnit> MapReinforcementClass;
private:
	UFUNCTION()
	void SendReinforcements(const UTokenConsumer* target, int tokenAmount);
};
