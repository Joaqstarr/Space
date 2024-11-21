// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityVolume.generated.h"

class UShapeComponent;
/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGravityVolume : public UActorComponent
{
	GENERATED_BODY()
public:
	UGravityVolume();
	virtual FVector GetGravityDirection(const FVector& objectPos) const;

	int32 GetVolumePriority() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GravityStrength = 9;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 priority = 0;

	virtual void BeginPlay() override;
private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> PrimitiveVolume;
};
