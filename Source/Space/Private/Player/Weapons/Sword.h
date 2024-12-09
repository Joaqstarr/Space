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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnableDamage();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisableDamage();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Stats")
	float Damage = 5;
	UPROPERTY(EditAnywhere, Category="Weapon Stats")
	float SphereTraceRadius = 100;

	UPROPERTY(EditAnywhere)
	bool bDebugDraw = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> SwordMesh;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsDamageEnabled = false;


protected:
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY()
	TArray<AActor*> IgnoredActors;
private:
	void SweepAndApplyDamage();

};
