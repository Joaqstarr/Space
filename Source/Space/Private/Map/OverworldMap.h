// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OverworldMap.generated.h"

class UMapTransformComponent;
class UMapObject;
class UBoxComponent;
class IMapObject;

UCLASS()
class AOverworldMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOverworldMap();
	virtual void Tick(float DeltaTime) override;

	//Adds map object to array
	void AddMapObject(AActor* mapObject);
	void RemoveMapObject(AActor* mapObject);

	FVector WorldspaceToMapSpace(const FVector& worldPos) const;
	FVector MapSpaceToWorldSpace(const FVector& mapSpace) const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
protected:
	//holds every map object within this map. Must implement IMapObject.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TArray<TObjectPtr<AActor>> MapObjects;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> MapBounds;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	
};
