// Fill out your copyright notice in the Description page of Project Settings.

#include "TokenConsumer.h"
#include "Utility/TokenTransferUnit.h"
#include "GameManagers/Subsystems/EnemyManagerWorldSubsystem.h"

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

void UTokenConsumer::UnregisterConsumer()
{
	Manager->UnregisterTokenConsumer(this);
}

void UTokenConsumer::NotifyTransferRequested(const UTokenConsumer* Target, int TokenAmount) const
{
	UE_LOG(LogTemp, Log, TEXT("Consumer %s is sending %d reinforcements to %s."), *GetConsumerName(), TokenAmount, *Target->GetConsumerName());
	OnTransferSent.Broadcast(Target, TokenAmount);
}

void UTokenConsumer::OnReceiveTransfer(TScriptInterface<ITokenTransferUnit> TransferUnit) const
{
	if (TransferUnit.GetObject() == nullptr || !TransferUnit.GetObject()->Implements<UTokenTransferUnit>())
	{
		UE_LOG(LogTemp, Error, TEXT("TokenConsumer OnReceiveTransfer: TransferUnit provided does not implement ITokenTransferUnit"));
		return;
	}

	if (Manager->GetTransferDestination(TransferUnit) == this)
	{
		Manager->FinalizeTransfer(TransferUnit);
	}
}

void UTokenConsumer::RegisterTransfer(TScriptInterface<ITokenTransferUnit> TransferUnit, const UTokenConsumer* Target, int TokenAmount) const
{
	if (TransferUnit.GetObject() == nullptr || !TransferUnit.GetObject()->Implements<UTokenTransferUnit>())
	{
		UE_LOG(LogTemp, Error, TEXT("TokenConsumer RegisterTransfer: TransferUnit provided does not implement ITokenTransferUnit %p %d"), TransferUnit.GetObject(), TransferUnit.GetObject()->Implements<UTokenTransferUnit>());
		return;
	}
	Manager->RegisterTokenTransfer(TransferUnit, this, Target, TokenAmount);
}
