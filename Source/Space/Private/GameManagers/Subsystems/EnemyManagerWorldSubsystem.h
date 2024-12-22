// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyManagerWorldSubsystem.generated.h"

class UTokenConsumer;

struct FConsumerInfo
{
	int TokensAllocated;
	int TokensInbound;
	int Priority;
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
	*	Registers an object that should be allocated tokens for spawning purposes (for example, enemy bases)
	*   @return true if Consumer was successfully registered
	*/
	bool RegisterTokenConsumer(UTokenConsumer* Consumer, int Priority);

	/* 
	*	Unregisters a consumer. Any tokens assigned to it will be reallocated to remaining consumers.
	*   @return true if Consumer was successfully unregistered
	*/
	bool UnregisterTokenConsumer(UTokenConsumer* Consumer);

	/* 
	*	Gets the number of tokens assigned to the consumer with the specified id.
	*	If the specified id is unregistered, returns 0.
	*/
	UFUNCTION(BlueprintCallable)
	int GetAssignedTokens(const UTokenConsumer* Consumer) const;

	UFUNCTION(BlueprintCallable)
	void SetConsumerPriority(const UTokenConsumer* Consumer, int Priority);

	UFUNCTION(BlueprintCallable)
	int GetConsumerPriority(const UTokenConsumer* Consumer) const;

	/*
	*	Requests for more tokens to be reallocated to a consumer from other consumers in the system
	*	@param ConsumerId Consumer to allocate more tokens to
	*	@param Tokens Target number of tokens to give to the consumer
	*	@return Number of tokens that were able to be given to the consumer
	*/
	UFUNCTION(BlueprintCallable)
	int RequestAdditionalTokens(const UTokenConsumer* Consumer, int Tokens);

	/*
	*	Distributes all available tokens among all consumers.
	*	Consumers with higher priority will be allocated more tokens.
	*	@param Tokens Number of tokens to distribute
	*/
	UFUNCTION(BlueprintCallable)
	void DistributeTokens(int Tokens, bool IsInstantTransmission);

	/*
	*	Finalizes the transfer of tokens that were in transit by adding it to the consumer's allocation pool.
	*	@param TargetId The consumer receiving the token payload
	*	@param Amount Number of tokens in the payload
	*/
	UFUNCTION(BlueprintCallable)
	void FinalizeTransfer(const UTokenConsumer* Target, int Amount);

	UFUNCTION(BlueprintCallable)
	void DebugPrintState();

private:
	int TotalTokens;

	// Starts a transfer between two consumers
	void TransferTokens(const UTokenConsumer* Source, const UTokenConsumer* Target, int Amount);

	// Adds tokens to a target's allocated pool, either directly or through the transit
	void SendTokens(const UTokenConsumer* Target, int Amount, bool IsInstantTransmission);

	// Maps consumers to the number of tokens currently held by that consumer
	TMap<UTokenConsumer*, FConsumerInfo> Consumers;
};
