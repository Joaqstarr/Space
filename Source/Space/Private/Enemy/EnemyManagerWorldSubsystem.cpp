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

void UEnemyManagerWorldSubsystem::SetTokens(int Amount)
{
	TotalTokens = Amount;
}

int UEnemyManagerWorldSubsystem::GetTotalTokens() const
{
	return TotalTokens;
}

void UEnemyManagerWorldSubsystem::ModifyTokens(int Amount)
{
	TotalTokens = FMath::Max(0, TotalTokens + Amount);
}

bool UEnemyManagerWorldSubsystem::RegisterTokenConsumer(int ConsumerId, int Priority)
{
	bool IsDuplicate = Consumers.Contains(ConsumerId);
	if (IsDuplicate)
	{
		TotalPriority += Priority - Consumers[ConsumerId].Priority;
		Consumers[ConsumerId].Priority = Priority;
	}
	else
	{
		TotalPriority += Priority;
		Consumers.Emplace(ConsumerId, {ConsumerId, Priority, 0});
	}

	AllocateTokens();

	return !IsDuplicate;
}

bool UEnemyManagerWorldSubsystem::UnregisterTokenConsumer(int ConsumerId)
{
	if (Consumers.Contains(ConsumerId))
	{
		TotalPriority -= Consumers[ConsumerId].Priority;
		Consumers.Remove(TotalPriority);

		AllocateTokens();

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

void UEnemyManagerWorldSubsystem::AllocateTokens()
{
	if (TotalPriority == 0)
	{
		return;
	}

	TArray<int> Keys;
	Consumers.GetKeys(Keys);

	int TotalTokensAllocated = 0;
	for (const auto& Key : Keys)
	{
		int NumTokens = FMath::Floor(static_cast<float>(Consumers[Key].Priority) / TotalPriority * TotalTokens);

		TotalTokensAllocated += NumTokens;
		Consumers[Key].TokensAllocated = NumTokens;
	}

	int TokensLeftover = TotalTokens - TotalTokensAllocated;

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

	for (int i = 0; i < TokensLeftover; ++i)
	{
		Consumers[Keys[i]].TokensAllocated++;
	}
}
