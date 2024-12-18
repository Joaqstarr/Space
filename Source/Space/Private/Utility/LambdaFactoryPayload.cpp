// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/LambdaFactoryPayload.h"

void ULambdaFactoryPayload::Bind(TFunction<UObject* ()> Func)
{
	FactoryFunction = Func;
}

UObject* ULambdaFactoryPayload::CreateInstance_Implementation() const
{
	return FactoryFunction ? FactoryFunction() : nullptr;
}