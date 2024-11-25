// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PoolManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPoolManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPoolManagerComponent();

	UFUNCTION(BlueprintCallable, Category=Pooling)
	AActor* GetPooledActor();

	UFUNCTION(BlueprintCallable, Category=Pooling)
	void ReturnPooledActor(AActor* actor);

	UFUNCTION(BlueprintCallable, Category=Pooling)
	TSubclassOf<AActor> GetClassToPool();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly, Category=Pooling)
	TSubclassOf<AActor> ClassToPool;

	UPROPERTY(EditDefaultsOnly, Category=Pooling)
	int32 PoolSize = 20;

	UPROPERTY()
	TArray<AActor*> Pool;

	void InitializePool();
};
