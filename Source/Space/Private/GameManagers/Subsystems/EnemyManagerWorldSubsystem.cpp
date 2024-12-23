// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManagers/Subsystems/EnemyManagerWorldSubsystem.h"
#include "Components/TokenConsumer.h"

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

bool UEnemyManagerWorldSubsystem::RegisterTokenConsumer(UTokenConsumer* Consumer, int Priority)
{
	if (!Consumer || Consumers.Contains(Consumer))
	{
		return false;
	}

	Consumers.Add(Consumer, {0, 0, Priority});
	return true;
}

bool UEnemyManagerWorldSubsystem::UnregisterTokenConsumer(UTokenConsumer* Consumer)
{
	if (Consumers.Contains(Consumer))
	{
		DistributeTokensFromSource(Consumers[Consumer].TokensAllocated, Consumer);
		Consumers.Remove(Consumer);
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

	// Calculate total priority
	int TotalPriority = 0;
	for (const auto& Consumer : Consumers)
	{
		if (IgnoreList.Contains(Consumer.Key)) { continue; }
		TotalPriority += Consumer.Value.Priority;
	}

	if (TotalPriority == 0) { return {}; }

	// Distribute tokens proportionally by priority. Higher priority consumers get more tokens
	TMap<const UTokenConsumer*, int> Result{};
	int TotalTokensAllocated = 0;

	for (auto& Consumer : Consumers)
	{
		if (IgnoreList.Contains(Consumer.Key)) { continue; }

		float NumTokens = FMath::Floor(static_cast<float>(Consumer.Value.Priority) / TotalPriority * Tokens);

		TotalTokensAllocated += NumTokens;
		Result.Add(Consumer.Key, NumTokens);
	}

	// Handle leftovers due to rounding
	int TokensLeftover = Tokens - TotalTokensAllocated;

	if (TokensLeftover == 0) { return Result; }

	// Assign leftovers by descending priority
	Consumers.ValueSort(
		[](const FConsumerInfo& a, const FConsumerInfo& b)
		{
			return a.Priority > b.Priority;
		}
	);
	
	for (auto Iter = Consumers.CreateIterator(); TokensLeftover > 0; ++Iter, --TokensLeftover)
	{
		if (!IgnoreList.Contains(Iter.Key()))
		{
			++Result[Iter.Key()];
		}
	}

	return Result;
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
	if (Source == nullptr || !Consumers.Contains(Source)) { return; }
	Tokens = FMath::Min(Tokens, Consumers[Source].TokensAllocated);

	TMap<const UTokenConsumer*, int> Distribution = GetDistribution(Tokens, { Source });

	// Tokens could not be distributed
	if (Distribution.Num() == 0)
	{
		UnallocatedTokens += Tokens;
		return;
	}

	for (auto& Consumer : Distribution)
	{
		TransferTokens(Source, Consumer.Key, Consumer.Value);
	}
}

void UEnemyManagerWorldSubsystem::TransferTokens(const UTokenConsumer* Source, const UTokenConsumer* Target, int Amount)
{
	if (Source == nullptr || !Consumers.Contains(Source) || Amount <= 0) { return; }
	Consumers[Source].TokensAllocated -= Amount;

	Source->NotifyTransferSent(Target, Amount);

	SendTokens(Target, Amount, false);
}

void UEnemyManagerWorldSubsystem::SendTokens(const UTokenConsumer* Target, int Amount, bool IsInstantTransmission)
{
	if (Target == nullptr || !Consumers.Contains(Target)) { return; }
	if (IsInstantTransmission)
	{
		Consumers[Target].TokensAllocated += Amount;
	}
	else
	{
		Consumers[Target].TokensInbound += Amount;
	}
}

// TODO: Need to handle case where the target is destroyed while a payload is in transit
void UEnemyManagerWorldSubsystem::FinalizeTransfer(const UTokenConsumer* Target, int Amount)
{
	if (!Consumers.Contains(Target)) { return; }

	int AmountAvailable = FMath::Min(Consumers[Target].TokensInbound, Amount);

	Consumers[Target].TokensInbound -= AmountAvailable;
	Consumers[Target].TokensAllocated += AmountAvailable;
}

int UEnemyManagerWorldSubsystem::RequestAdditionalTokens(const UTokenConsumer* Target, int RequestedTokens)
{
	// Invalid request
	if (Target == nullptr || !Consumers.Contains(Target) || RequestedTokens <= 0) { return 0; }

	// No available tokens remain in the system
	int AvailableTokens = TotalTokens - Consumers[Target].TokensAllocated;
	if (AvailableTokens == 0) { return 0; }


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

	int TokensLeftover = RequestedTokens - TokensContributed;

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
	FString Output = FString::Printf(TEXT("Enemy Manager Subsystem State:\n Total Tokens: %d\n"), TotalTokens);
	for (const auto& Consumer : Consumers)
	{
		Output += FString::Printf(TEXT("Consumer %d with priority %d has %d tokens held and %d tokens inbound.\n"), Consumer.Key->GetUniqueID(), Consumer.Value.Priority, Consumer.Value.TokensAllocated, Consumer.Value.TokensInbound);
	}
	Output += "\n";

	UE_LOG(LogTemp, Log, TEXT("%s"), *Output);
}
