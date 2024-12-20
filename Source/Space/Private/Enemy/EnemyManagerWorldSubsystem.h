// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyManagerWorldSubsystem.generated.h"

struct FTokenConsumer
{
	int Id;
	int Priority;
	int TokensAllocated;
};

/**
 * 
 */
UCLASS()
class UEnemyManagerWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/* 
	*	Assigns a certain number of spawning tokens to the enemy manager.
	*	Tokens represent how many enemies are allowed to be spawned.
	*/
	UFUNCTION(BlueprintCallable)
	void SetTokens(int Amount);

	/* Gets the total number of tokens currently held by the enemy manager. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetTotalTokens() const;

	/*
		Changes the amount of tokens held by the enemy manager. Final token amount is clamped to be positive.
		@param Amount The number of tokens that is added. Can be negative to subtract tokens.
	*/
	UFUNCTION(BlueprintCallable)
	void ModifyTokens(int Amount);

	/* 
	*	Registers an object that should be allocated tokens for spawning purposes (for example, enemy bases)
	*	@param ConsumerId A number that uniquely identifies this consumer. If the id is already registered, its information
	*   will be updated.
	*   @param Priority Consumers with higher priority will be assigned more tokens
	*   @return true if ConsumerId was not previously registered
	*/
	UFUNCTION(BlueprintCallable)
	bool RegisterTokenConsumer(int ConsumerId, int Priority);

	/* 
	*	Unregisters a consumer. Any tokens assigned to it will be reallocated to remaining consumers.
	*   @return true if ConsumerId was successfully unregistered
	*/
	UFUNCTION(BlueprintCallable)
	bool UnregisterTokenConsumer(int ConsumerId);

	/* 
	*	Gets the number of tokens assigned to the consumer with the specified id.
	*	If the specified id is unregistered, returns 0.
	*/
	UFUNCTION(BlueprintCallable)
	int GetAssignedTokens(int ConsumerId) const;

private:
	int TotalTokens;
	int TotalPriority;
	TMap<int, FTokenConsumer> Consumers;

	void AllocateTokens();
};
