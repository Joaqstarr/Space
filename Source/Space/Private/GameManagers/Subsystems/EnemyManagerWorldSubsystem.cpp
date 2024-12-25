// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManagers/Subsystems/EnemyManagerWorldSubsystem.h"
#include "Components/TokenConsumer.h"
#include "Utility/TokenTransferUnit.h"

static bool IsHigherPriority(const FConsumerInfo& A, const FConsumerInfo& B);

void UEnemyManagerWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UEnemyManagerWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

int UEnemyManagerWorldSubsystem::GetTotalTokens() const
{
	return TotalTokens;
}

bool UEnemyManagerWorldSubsystem::RegisterTokenConsumer(const UTokenConsumer* Consumer, int Priority)
{
	if (!Consumer || Consumers.Contains(Consumer))
	{
		return false;
	}

	Consumers.Add(Consumer, FConsumerInfo{Priority});
	return true;
}

bool UEnemyManagerWorldSubsystem::UnregisterTokenConsumer(const UTokenConsumer* Consumer)
{
	if (Consumers.Contains(Consumer))
	{
		DistributeTokensFromSource(Consumers[Consumer].TokensAllocated, Consumer);
		Consumers.Remove(Consumer);
		RerouteInboundTransfers(Consumer);
		return true;
	}
	return false;
}

int UEnemyManagerWorldSubsystem::GetAssignedTokens(const UTokenConsumer* Consumer) const
{
	if (!Consumers.Contains(Consumer))
	{
		return 0;
	}
	return Consumers[Consumer].TokensAllocated;
}

void UEnemyManagerWorldSubsystem::SetConsumerPriority(const UTokenConsumer* Consumer, int Priority)
{
	if (Consumers.Contains(Consumer))
	{
		Consumers[Consumer].Priority = FMath::Max(Priority, 0);
	}
}

int UEnemyManagerWorldSubsystem::GetConsumerPriority(const UTokenConsumer* Consumer) const
{
	if (Consumers.Contains(Consumer))
	{
		return Consumers[Consumer].Priority;
	}
	return 0;
}

TMap<const UTokenConsumer*, int> UEnemyManagerWorldSubsystem::GetDistribution(int Tokens, const TArray<const UTokenConsumer*>& IgnoreList)
{
	if (Tokens <= 0) { return {}; }

	TMap<const UTokenConsumer*, FConsumerInfo> Recipients = Consumers.FilterByPredicate(
		[&IgnoreList](const TPair<const UTokenConsumer*, FConsumerInfo>& Consumer)
		{
			return !IgnoreList.Contains(Consumer.Key);
		}
	);

	// Calculate total priority
	int TotalPriority = 0;
	for (const auto& Consumer : Recipients)
	{
		TotalPriority += Consumer.Value.Priority;
	}

	if (TotalPriority == 0) { return {}; }

	// Distribute tokens proportionally by priority. Higher priority consumers get more tokens
	TMap<const UTokenConsumer*, int> Result{};
	int TotalTokensAllocated = 0;

	for (auto& Consumer : Recipients)
	{
		float NumTokens = FMath::Floor(static_cast<float>(Consumer.Value.Priority) / TotalPriority * Tokens);

		TotalTokensAllocated += NumTokens;
		Result.Add(Consumer.Key, NumTokens);
	}

	// Handle leftovers due to rounding
	int TokensLeftover = Tokens - TotalTokensAllocated;

	if (TokensLeftover == 0) { return Result; }

	// Assign leftovers by descending priority
	Recipients.ValueSort(
		[](const FConsumerInfo& A, const FConsumerInfo& B)
		{
			return A.Priority > B.Priority;
		}
	);
	
	// Number of leftover tokens should always be less than the number of consumers
	for (auto Iter = Recipients.CreateIterator(); TokensLeftover > 0; ++Iter, --TokensLeftover)
	{
		++Result[Iter.Key()];
	}
	return Result;
}

void UEnemyManagerWorldSubsystem::RerouteInboundTransfers(const UTokenConsumer* Target)
{
	auto InboundTransfers = Transfers.FilterByPredicate(
		[=](const TPair<TScriptInterface<ITokenTransferUnit>, FTransferInfo>& Transfer)
		{
			return Transfer.Key.GetObject() != nullptr && Transfer.Value.Destination == Target;
		}
	);

	// Find a fallback consumer to send transfers to if both the source and the dest are destroyed
	const TPair<const UTokenConsumer*, FConsumerInfo>* Fallback{};
	for (const auto& Consumer : Consumers)
	{
		if (!Fallback || IsHigherPriority(Consumer.Value, Fallback->Value))
		{
			Fallback = &Consumer;
		}
	}

	const UTokenConsumer* Destination;
	for (const auto& Transfer : InboundTransfers)
	{
		// Send the transfers back to where they came from, if possible
		if (Consumers.Contains(Transfer.Value.Source))
		{
			Destination = Transfer.Value.Source;
		}
		else if (Fallback)
		{
			Destination = Fallback->Key;
		}
		else
		{
			// This should only happen if all bases get destroyed and there is an active transfer
			// TODO: Figure out a good way to handle this case
			Destination = nullptr;
		}
		
		Transfers[Transfer.Key].Destination = Destination;
		ITokenTransferUnit::Execute_OnTargetChange(Transfer.Key.GetObject(), Destination);

		if (Consumers.Contains(Destination))
		{
			Consumers[Destination].TokensInbound += Transfer.Value.TokenAmount;
		}
	}
	if (Consumers.Contains(Target))
	{
		Consumers[Target].TokensInbound = 0;
	}
}

void UEnemyManagerWorldSubsystem::DistributeTokensInstant(int Tokens)
{
	TMap<const UTokenConsumer*, int> Distribution = GetDistribution(Tokens, {});

	for (auto& Consumer : Distribution)
	{
		SendTokens(Consumer.Key, Consumer.Value, true);
	}
}

void UEnemyManagerWorldSubsystem::DistributeTokensFromSource(int Tokens, const UTokenConsumer* Source)
{
	if (!Source || !Consumers.Contains(Source)) { return; }
	Tokens = FMath::Min(Tokens, Consumers[Source].TokensAllocated);

	TMap<const UTokenConsumer*, int> Distribution = GetDistribution(Tokens, { Source });

	// Tokens could not be distributed - all other bases have been destroyed
	if (Distribution.Num() == 0)
	{
		UnallocatedTokens += Tokens;
		return;
	}

	int Test{};
	for (auto& Consumer : Distribution)
	{
		TransferTokens(Source, Consumer.Key, Consumer.Value);
		Test += Consumer.Value;
	}
}

void UEnemyManagerWorldSubsystem::TransferTokens(const UTokenConsumer* Source, const UTokenConsumer* Target, int Amount)
{
	if (!Source || !Consumers.Contains(Source) || Amount <= 0) { return; }
	Consumers[Source].TokensAllocated -= Amount;
	TokensInTransfer += Amount;

	Source->NotifyTransferRequested(Target, Amount);

	SendTokens(Target, Amount, false);
}

void UEnemyManagerWorldSubsystem::SendTokens(const UTokenConsumer* Target, int Amount, bool IsInstantTransmission)
{
	if (!Target || !Consumers.Contains(Target)) { return; }
	if (IsInstantTransmission)
	{
		Consumers[Target].TokensAllocated += Amount;
	}
	else
	{
		Consumers[Target].TokensInbound += Amount;
	}
}

void UEnemyManagerWorldSubsystem::FinalizeTransfer(TScriptInterface<ITokenTransferUnit> TransferUnit)
{
	// Invalid transfer unit
	if (!Transfers.Contains(TransferUnit)) 
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyManagerSubsystem FinalizeTransfer: Invalid transfer."));
		return;
	}

	const UTokenConsumer* Destination = Transfers[TransferUnit].Destination;
	if (!Consumers.Contains(Destination))
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyManagerSubsystem FinalizeTransfer: Invalid destination"))
		return;
	}

	int TokenAmount = Transfers[TransferUnit].TokenAmount;

	Consumers[Destination].TokensInbound -= TokenAmount;
	Consumers[Destination].TokensAllocated += TokenAmount;

	TokensInTransfer -= TokenAmount;

	ITokenTransferUnit::Execute_OnTransferComplete(TransferUnit.GetObject());
	Transfers.Remove(TransferUnit);
}

void UEnemyManagerWorldSubsystem::RegisterTokenTransfer(TScriptInterface<ITokenTransferUnit> TransferUnit, const UTokenConsumer* Source, const UTokenConsumer* Destination, int TokenAmount)
{
	Transfers.Add(TransferUnit, FTransferInfo{ Source, Destination, TokenAmount });
	ITokenTransferUnit::Execute_OnTargetChange(TransferUnit.GetObject(), Destination);
}


int UEnemyManagerWorldSubsystem::RequestAdditionalTokens(const UTokenConsumer* Target, int RequestedTokens)
{
	// Invalid request
	if (!Target || !Consumers.Contains(Target) || RequestedTokens <= 0) { return 0; }

	// No contributors
	if (Consumers.Num() == 0 || Consumers.Num() == 1)
	{
		return 0;
	}

	// There is only one possible contributor
	if (Consumers.Num() == 2)
	{
		for (auto& Consumer : Consumers)
		{
			if (Consumer.Key != Target)
			{
				int Contribution = FMath::Min(Consumer.Value.TokensAllocated, RequestedTokens);

				TransferTokens(Consumer.Key, Target, Contribution);

				return Contribution;
			}
		}
	}

	// Calculate priority for all contributors
	int TotalPriority = 0;
	for (const auto& Consumer : Consumers)
	{
		TotalPriority += Consumer.Value.Priority;
	}

	// Requesting consumer is not a contributor
	TotalPriority -= Consumers[Target].Priority;

	TMap<const UTokenConsumer*, int> ContributionSplit;
	int TokensContributed = 0;

	// Calculate number of tokens that each consumer will contribute to the request
	for (const auto& Consumer : Consumers)
	{
		if (Consumer.Key == Target) { continue; }
		
		// Consumers with lower priority should contribute more
		float InversePriority = static_cast<float>(TotalPriority - Consumer.Value.Priority) / (Consumers.Num() - 2);
		int Contribution = FMath::Floor(InversePriority / TotalPriority * RequestedTokens);

		TokensContributed += Contribution;
		ContributionSplit.Add(Consumer.Key, Contribution);
	}

	// There may be leftovers due to rounding
	int TokensLeftover = RequestedTokens - TokensContributed;

	// Assign leftover tokens by ascending priority and ascending token count
	Consumers.ValueSort(
		[](const FConsumerInfo& a, const FConsumerInfo& b)
		{
			if (a.Priority == b.Priority)
			{
				return a.TokensAllocated < b.TokensAllocated;
			}
			return a.Priority < b.Priority;
		}
	);

	// Handle leftovers and transfer tokens
	for (const auto& Consumer : Consumers)
	{
		if (Consumer.Key == Target) { continue; }

		int TargetCont = ContributionSplit[Consumer.Key] + TokensLeftover;
		int ActualCont = FMath::Min(TargetCont, Consumer.Value.TokensAllocated);

		TransferTokens(Consumer.Key, Target, ActualCont);

		TokensLeftover = TargetCont - ActualCont;
	}

	// If there are still tokens leftover, then there weren't enough in the system
	return RequestedTokens - TokensLeftover;
}

void UEnemyManagerWorldSubsystem::DebugPrintState()
{
	FString Output{" \n\n=====Enemy Manager World Subsystem State=====\n"};
	int AllocatedTotal{};
	for (const auto& Consumer : Consumers)
	{
		Output += FString::Printf(TEXT("Consumer %d with priority %d has %d tokens held and %d tokens inbound.\n"), Consumer.Key->GetUniqueID(), Consumer.Value.Priority, Consumer.Value.TokensAllocated, Consumer.Value.TokensInbound);
		AllocatedTotal += Consumer.Value.TokensAllocated;
	}
	Output += FString::Printf(TEXT("\nTotal Tokens Allocated: %d\n"), AllocatedTotal);
	Output += "\n\nTransfers:\n";

	int TransferTotal{};
	for (const auto& Transfer : Transfers)
	{
		Output += FString::Printf(TEXT("%d going from %s to %s\n"), Transfer.Value.TokenAmount, *Transfer.Value.Source->GetConsumerName(), *Transfer.Value.Destination->GetConsumerName());
		TransferTotal += Transfer.Value.TokenAmount;
	}
	Output += FString::Printf(TEXT("\nTokens In Transfer: %d\n"), TransferTotal);
	Output += "=============================================";
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Output);
}

const UTokenConsumer* UEnemyManagerWorldSubsystem::GetTransferDestination(TScriptInterface<ITokenTransferUnit> TransferUnit)
{
	if (Transfers.Contains(TransferUnit))
	{
		return Transfers[TransferUnit].Destination;
	}
	return nullptr;
}

// Helper function that returns true if a is higher priority than b
static bool IsHigherPriority(const FConsumerInfo& A, const FConsumerInfo& B)
{
	if (A.Priority == B.Priority)
	{
		return A.TokensAllocated < B.TokensAllocated;
	}
	return A.Priority > B.Priority;
}
