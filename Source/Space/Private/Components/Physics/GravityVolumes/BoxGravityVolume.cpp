// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGravityVolume.h"
#include "Components/BoxComponent.h"

UBoxGravityVolume::UBoxGravityVolume()
{
	AsBoxComponent = CreateDefaultSubobject<UBoxComponent>("Box Volume");
	PrimitiveVolume = AsBoxComponent;
	PrimitiveVolume->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}

FVector UBoxGravityVolume::GetGravityDirection(const FVector& objectPos) const
{
	FVector down {-PrimitiveVolume->GetUpVector()};
	return down * GravityStrength;
}
