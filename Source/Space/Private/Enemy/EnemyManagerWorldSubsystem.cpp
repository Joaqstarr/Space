// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/EnemyManagerWorldSubsystem.h"

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

void UEnemyManagerWorldSubsystem::ModifyTokens(int Amount)
{
	TotalTokens = FMath::Max(0, TotalTokens + Amount);
}

bool UEnemyManagerWorldSubsystem::RegisterTokenConsumer(int ConsumerId, int Priority, FOnTokensAllocated Callback)
{
	bool IsDuplicate = Consumers.Contains(ConsumerId);
	if (IsDuplicate)
	{
		Consumers[ConsumerId].Priority = Priority;
		Consumers[ConsumerId].Callback = Callback;
	}
	else
	{
		Consumers.Emplace(ConsumerId, {ConsumerId, Priority, 0, Callback});
	}

	return !IsDuplicate;
}

bool UEnemyManagerWorldSubsystem::UnregisterTokenConsumer(int ConsumerId)
{
	if (Consumers.Contains(ConsumerId))
	{
		int TokensHeld = Consumers[ConsumerId].TokensAllocated;
		TotalTokens -= TokensHeld;

		Consumers.Remove(ConsumerId);

		DistributeTokens(TokensHeld, false);

		return true;
	}
	return false;
}

int UEnemyManagerWorldSubsystem::GetAssignedTokens(int ConsumerId) const
{
	if (!Consumers.Contains(ConsumerId))
	{
		return 0;
	}
	return Consumers[ConsumerId].TokensAllocated;
}

void UEnemyManagerWorldSubsystem::DistributeTokens(int Tokens, bool IsInstantTransmission)
{
	if (Tokens <= 0) { return; }

	TotalTokens += Tokens;

	int TotalPriority = 0;
	for (const auto& Consumer : Consumers)
	{
		TotalPriority += Consumer.Value.Priority;
	}

	if (TotalPriority == 0) { return; }

	TMap<int, int> TokensDistributed{};

	int TotalTokensAllocated = 0;
	for (auto& Consumer : Consumers)
	{
		int NumTokens = FMath::Floor(static_cast<float>(Consumer.Value.Priority) / TotalPriority * Tokens);

		TotalTokensAllocated += NumTokens;
		TokensDistributed.Emplace(Consumer.Value.Id, NumTokens);
	}

	int TokensLeftover = Tokens - TotalTokensAllocated;

	if (TokensLeftover == 0)
	{
		return;
	}

	// Assign leftover tokens by descending priority
	Consumers.ValueSort(
		[](const FTokenConsumer& a, const FTokenConsumer& b)
		{
			return a.Priority > b.Priority;;
		}
	);

	int LeftoversAssigned = 0;
	for (auto& Consumer : Consumers)
	{
		if (LeftoversAssigned == TokensLeftover)
		{
			break;
		}
		++TokensDistributed[Consumer.Value.Id];
		++LeftoversAssigned;
	}

	// Notify all consumers of their newly allocated tokens
	for (const auto& Consumer : Consumers)
	{
		SendTokens(Consumer.Value.Id, TokensDistributed[Consumer.Value.Id], IsInstantTransmission);
	}
}

void UEnemyManagerWorldSubsystem::DebugPrintState()
{
	FString Output = FString::Printf(TEXT("Enemy Manager Subsystem State:\n Total Tokens: %d\n"), TotalTokens);
	for (const auto& Consumer : Consumers)
	{
		Output += FString::Printf(TEXT("Consumer %d with priority %d has %d tokens.\n"), Consumer.Value.Id, Consumer.Value.Priority, Consumer.Value.TokensAllocated);
	}
	UE_LOG(LogTemp, Log, TEXT("%s"), *Output);
}

// TODO: Eventually add this to some in between buffer to represent the "travel time" of reinforcements
void UEnemyManagerWorldSubsystem::TransferTokens(int SourceId, int TargetId, int Amount)
{
	Consumers[SourceId].TokensAllocated -= Amount;
	Consumers[TargetId].TokensAllocated += Amount;
}

void UEnemyManagerWorldSubsystem::SendTokens(int TargetId, int Amount, bool IsInstantTransmission)
{
	if (IsInstantTransmission)
	{
		Consumers[TargetId].TokensAllocated += Amount;
		return;
	}

	if (TokensInTransit.Contains(TargetId))
	{
		TokensInTransit[TargetId] += Amount;
	}
	else
	{
		TokensInTransit.Emplace(TargetId, Amount);
	}
}

// TODO: Need to handle case where the target is destroyed while a payload is in transit
void UEnemyManagerWorldSubsystem::FinalizeTransfer(int TargetId, int Amount)
{
	if (!TokensInTransit.Contains(TargetId) || !Consumers.Contains(TargetId)) { return; }

	int AmountAvailable = FMath::Min(TokensInTransit[TargetId], Amount);
	TokensInTransit[TargetId] -= AmountAvailable;
	Consumers[TargetId].TokensAllocated += AmountAvailable;
}

int UEnemyManagerWorldSubsystem::RequestAdditionalTokens(int TargetId, int RequestedTokens)
{
	// Invalid request
	if (!Consumers.Contains(TargetId) || RequestedTokens <= 0) { return 0; }

	// No available tokens remain in the system
	int AvailableTokens = TotalTokens - Consumers[TargetId].TokensAllocated;
	if (AvailableTokens == 0) { return 0; }

	// Calculate priority for all contributors
	int TotalPriority = 0;
	int HighestPriority = 0;
	for (const auto& Consumer : Consumers)
	{
		TotalPriority += Consumer.Value.Priority;
		if (Consumer.Value.Priority > HighestPriority)
		{
			HighestPriority = Consumer.Value.Priority;
		}
	}

	// Requesting consumer is not a contributor
	TotalPriority -= Consumers[TargetId].Priority;

	TMap<int, int> ContributionSplit;

	int TokensContributed = 0;


	// There is only one possible contributor
	if (Consumers.Num() == 2)
	{
		for (auto& Consumer : Consumers)
		{
			if (Consumer.Value.Id != TargetId)
			{
				int Contribution = FMath::Min(Consumer.Value.TokensAllocated, RequestedTokens);
				
				TransferTokens(Consumer.Value.Id, TargetId, Contribution);

				return Contribution;
			}
		}
	}

	// Calculate number of tokens that each consumer will contribute to the request
	for (const auto& Consumer : Consumers)
	{
		if (Consumer.Value.Id == TargetId) { continue; }
		
		// Consumers with lower priority should contribute more
		float InversePriority = static_cast<float>(TotalPriority - Consumer.Value.Priority) / (Consumers.Num() - 2);
		int Contribution = FMath::Floor(InversePriority / TotalPriority * RequestedTokens);

		TokensContributed += Contribution;
		ContributionSplit.Add(Consumer.Value.Id, Contribution);
	}

	// Assign leftover tokens by ascending priority
	Consumers.ValueSort(
		[](const FTokenConsumer& a, const FTokenConsumer& b)
		{
			return a.Priority < b.Priority;;
		}
	);

	int TokensLeftover = RequestedTokens - TokensContributed;

	// Handle leftovers and transfer tokens
	for (const auto& Consumer : Consumers)
	{
		if (Consumer.Value.Id == TargetId) { continue; }

		int TargetCont = ContributionSplit[Consumer.Value.Id] + TokensLeftover;
		int ActualCont = FMath::Min(TargetCont, Consumer.Value.TokensAllocated);

		TransferTokens(Consumer.Value.Id, TargetId, ActualCont);

		TokensLeftover = TargetCont - ActualCont;
	}

	// If there are still tokens leftover, then there weren't enough in the system
	return RequestedTokens - TokensLeftover;
}
