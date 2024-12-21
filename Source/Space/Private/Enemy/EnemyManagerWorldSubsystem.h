// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyManagerWorldSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnTokensAllocated, int, NumTokens);

struct FTokenConsumer
{
	int Id;
	int Priority;
	int TokensAllocated;
	FOnTokensAllocated Callback;
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

	/* Gets the total number of tokens currently held by the enemy manager. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int GetTotalTokens() const;

	/*
	*	Changes the amount of tokens held by the enemy manager. Final token amount is clamped to be positive.
	*	@param Amount The number of tokens that is added. Can be negative to subtract tokens.
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
	bool RegisterTokenConsumer(int ConsumerId, int Priority, FOnTokensAllocated Callback);

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

	/*
	*	Requests for more tokens to be reallocated to a consumer from other consumers in the system
	*	@param ConsumerId Consumer to allocate more tokens to
	*	@param Tokens Target number of tokens to give to the consumer
	*	@return Number of tokens that were able to be given to the consumer
	*/
	UFUNCTION(BlueprintCallable)
	int RequestAdditionalTokens(int ConsumerId, int Tokens);

	/*
	*	Distributes all available tokens among all consumers.
	*	Consumers with higher priority will be allocated more tokens.
	*	@param Tokens Number of tokens to distribute
	*/
	UFUNCTION(BlueprintCallable)
	void DistributeTokens(int Tokens);

	UFUNCTION(BlueprintCallable)
	void DebugPrintState();

private:
	int TotalTokens;
	TMap<int, FTokenConsumer> Consumers;

	void TransferTokens(int SourceId, int TargetId, int Amount);
};
