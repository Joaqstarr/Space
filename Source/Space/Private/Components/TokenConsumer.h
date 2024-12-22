// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TokenConsumer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTransferSentSignature, const UTokenConsumer*, Target, int, TokenAmount);

class UEnemyManagerWorldSubsystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTokenConsumer : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTokenConsumer();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnTransferSentSignature OnTransferSent;

	UFUNCTION(BlueprintGetter)
	int GetPriority() const;

	/*  Sets the priority for this token consumer
	*	@param Priority A non-negative value representing the priority of this consumer
	*/
	UFUNCTION(BlueprintSetter)
	void SetPriority(int Priority);

	/*
	*	Returns the number of tokens currently assigned to this consumer by the manager
	*/
	UFUNCTION(BlueprintCallable)
	int GetTokens() const;

	/*
	*	Alerts the consumer that it has had some of its tokens reallocated to another target
	*/
	void NotifyTransferSent(const UTokenConsumer* Target, int TokenAmount) const;

	/*
	*	Finalizes a transfer of inbound tokens.
	*	Call this when reinforcements arrive at a consumer.
	*/
	UFUNCTION(BlueprintCallable)
	void OnReceiveTransfer(int Amount) const;

	/*
	*	Requests more tokens from other consumers in the system.
	*	@return Number of tokens that are enroute
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	int RequestTokens(int Amount) const;

	UFUNCTION(BlueprintCallable)
	FString GetConsumerName() const;

protected:
	virtual void BeginPlay() override;

private:
	static int NextId;

	// This is only a cached variable and not the actual priority used
	UPROPERTY(EditInstanceOnly, BlueprintGetter = GetPriority, BlueprintSetter = SetPriority)
	int Priority;

	TObjectPtr<UEnemyManagerWorldSubsystem> Manager;
};