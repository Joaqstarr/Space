// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Utility/PriorityQueue.h"
#include "GravityComponent.generated.h"


struct FGameplayTag;
class UGravityVolume;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGravityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGravityComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void EnteredGravityVolume(UGravityVolume* volume);
	void ExitedGravityVolume(UGravityVolume* volume);


	FVector GetGravityDirection() const;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector DefaultGravityDirection;
private:
	void ApplyGravity() const;
	TPriorityQueue<UGravityVolume*> GravityZonesQueue;
	FGameplayTag GravityTag;
	void RemoveGravityTag(const IAbilitySystemInterface& abilityOwner) const;
	void AddGravityTag(const IAbilitySystemInterface& abilityOwner) const;
};
