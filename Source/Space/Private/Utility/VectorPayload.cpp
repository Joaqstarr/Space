#include "VectorPayload.h"

#include "Net/UnrealNetwork.h"

bool UVectorPayload::IsSupportedForNetworking() const
{
	return true;
}

void UVectorPayload::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UVectorPayload, VectorData);
}
