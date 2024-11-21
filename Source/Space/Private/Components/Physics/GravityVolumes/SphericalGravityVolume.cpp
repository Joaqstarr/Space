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
	float radius {AsSphereComponent->GetScaledSphereRadius()};
	float minDropoff = radius * GravityFalloffStart;
	float dist = dirLength.Length();

	float distInterp = ((dist-minDropoff) / (radius - minDropoff));
	distInterp = FMath::Clamp(distInterp, 0, 1);
	float interpTransformed = 1-distInterp;

	
	//TODO: Gravity falloff!

	
	FVector dir{dirLength.GetSafeNormal()};

	return dir * GravityStrength * interpTransformed;
}
