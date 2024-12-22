// Fill out your copyright notice in the Description page of Project Settings.

#include "TokenConsumer.h"
#include "GameManagers/Subsystems/EnemyManagerWorldSubsystem.h"

int UTokenConsumer::NextId = 0;

UTokenConsumer::UTokenConsumer()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTokenConsumer::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		Manager = GetWorld()->GetSubsystem<UEnemyManagerWorldSubsystem>();

		Manager->RegisterTokenConsumer(this, Priority);
	}
}


void UTokenConsumer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTokenConsumer::SetPriority(int NewPriority)
{
	Priority = FMath::Max(0, NewPriority);
	Manager->SetConsumerPriority(this, NewPriority);
}

int UTokenConsumer::GetTokens() const
{
	return Manager->GetAssignedTokens(this);
}

int UTokenConsumer::GetPriority() const
{
	return Manager->GetConsumerPriority(this);
}

int UTokenConsumer::RequestTokens(int Amount) const
{
	return Manager->RequestAdditionalTokens(this, Amount);
}

FString UTokenConsumer::GetConsumerName() const
{
	return GetOwner()->GetActorNameOrLabel();
}

void UTokenConsumer::NotifyTransferSent(const UTokenConsumer* Target, int TokenAmount) const
{
	UE_LOG(LogTemp, Log, TEXT("Consumer %s is sending %d reinforcements to %s."), *GetConsumerName(), TokenAmount, *Target->GetConsumerName());
	OnTransferSent.Broadcast(Target, TokenAmount);
}

void UTokenConsumer::OnReceiveTransfer(int TokenAmount) const
{
	Manager->FinalizeTransfer(this, TokenAmount);
}