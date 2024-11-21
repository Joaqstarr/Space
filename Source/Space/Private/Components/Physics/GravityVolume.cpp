// Fill out your copyright notice in the Description page of Project Settings.


#include "../Physics/GravityVolume.h"
#include "Components/Physics/GravityComponent.h"

UGravityVolume::UGravityVolume()
{
	PrimaryComponentTick.bCanEverTick = false;

	

}

FVector UGravityVolume::GetGravityDirection(const FVector& objectPos) const
{
	return FVector::DownVector * GravityStrength;
}

int32 UGravityVolume::GetVolumePriority() const
{
	return 1000-priority;
}

void UGravityVolume::BeginPlay()
{
	Super::BeginPlay();
	
	USceneComponent* root = GetOwner()->GetRootComponent();  // This could be a collision component like UBoxComponent or USphereComponent.
	if (root && root->IsA<UPrimitiveComponent>())
	{
		PrimitiveVolume = Cast<UPrimitiveComponent>(root);

		PrimitiveVolume->SetVisibility(false);
		
		PrimitiveVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);  
		PrimitiveVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		PrimitiveVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		PrimitiveVolume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}

	if(PrimitiveVolume)
	{
		PrimitiveVolume->OnComponentBeginOverlap.AddDynamic(this, &UGravityVolume::OnBeginOverlap);
		PrimitiveVolume->OnComponentEndOverlap.AddDynamic(this, &UGravityVolume::OnEndOverlap);
	}
}



void UGravityVolume::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGravityComponent* gravComponent = OtherActor->GetComponentByClass<UGravityComponent>();

	if(!gravComponent)return;

	gravComponent->EnteredGravityVolume(this);
	
}

void UGravityVolume::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UGravityComponent* gravComponent = OtherActor->GetComponentByClass<UGravityComponent>();
	if(!gravComponent)return;

	gravComponent->ExitedGravityVolume(this);
	
}

