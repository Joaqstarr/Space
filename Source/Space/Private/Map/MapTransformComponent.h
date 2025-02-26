// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MapTransformComponent.generated.h"

class AOverworldMap;
//allows attached actor to interface with the map
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UMapTransformComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMapTransformComponent();
	void SetAssociatedMap(AOverworldMap* newMap);
	AOverworldMap* GetAssociatedMap();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	/* float 0-1
	 *  X: Left - Right
	 *  Y: Back - Forward
	 *  Z: Bottom - Top
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Map", ReplicatedUsing=OnRep_MapPosition)
	FVector MapPosition;
	void UpdateWorldPosition(AOverworldMap* withMap);

	void SetMapPosition(FVector newMapPosition);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	//map this object is attached to
	UPROPERTY(EditInstanceOnly, Replicated, Category="Map")
	TObjectPtr<AOverworldMap> AssociatedMap;
private:
	UFUNCTION()
	void OnRep_MapPosition();
};
