// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Physics/GravityVolumes/SphericalGravityVolume.h"

#include "Components/SphereComponent.h"

USphericalGravityVolume::USphericalGravityVolume()
{
	AsSphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere Volume");
	PrimitiveVolume = AsSphereComponent;
	PrimitiveVolume->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
}

FVector USphericalGravityVolume::GetGravityDirection(const FVector& objectPos) const
{
	FVector dirLength {GetComponentLocation() - objectPos};
	float radius = AsSphereComponent->GetScaledSphereRadius();
	//TODO: Gravity falloff!
	FVector dir{dirLength};
	dir.Normalize();

	return dir * GravityStrength;
}
