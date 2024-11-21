// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GravityVolume.generated.h"

class UShapeComponent;
/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGravityVolume : public USceneComponent
{
	GENERATED_BODY()
public:
	UGravityVolume();
	virtual FVector GetGravityDirection(const FVector& objectPos) const;

	int32 GetVolumePriority() const;
	void SetupComponentCollisions(UPrimitiveComponent* component);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GravityStrength = 9;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 priority = 0;

	virtual void BeginPlay() override;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UPrimitiveComponent> PrimitiveVolume;
private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
