// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapObject.h"
#include "GameFramework/Actor.h"
#include "MapPlanet.generated.h"

struct FPlanetData;
class UMapTransformComponent;
class UStaticMeshComponent;

UCLASS()
class AMapPlanet : public AActor, public IMapObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapPlanet();

	virtual UMapTransformComponent* GetMapTransformComponent() override;
	virtual void Tick(float DeltaTime) override;
	void SetPlanetData(FPlanetData* data);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> PlanetMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UMapTransformComponent> MapTransformComponent;
private:
	FPlanetData* PlanetData;

};
