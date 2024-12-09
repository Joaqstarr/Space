// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Weapons/Sword.h"

#include "KismetTraceUtils.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DamageEvents.h"
// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
}

void ASword::DisableDamage_Implementation()
{
	bIsDamageEnabled = false;

}

void ASword::EnableDamage_Implementation()
{
	bIsDamageEnabled = true;
}

void ASword::EnableSword_Implementation()
{
	IgnoredActors.Empty();
}

void ASword::DisableSword_Implementation()
{
	IgnoredActors.Empty();
}




void ASword::SweepAndApplyDamage()
{
	FCollisionQueryParams collisionQueryParams;
	collisionQueryParams.AddIgnoredActor(this);
	collisionQueryParams.AddIgnoredActor(GetOwner());
	collisionQueryParams.AddIgnoredActors(IgnoredActors);

	TArray<FHitResult> results;
	FVector start = SwordMesh->GetSocketLocation(FName("TraceStart"));
	FVector end = SwordMesh->GetSocketLocation(FName("TraceEnd"));

	if(bDebugDraw)
	{
		DrawDebugSweptSphere(GetWorld(), start, end, SphereTraceRadius, FColor::Green, false, -1, 0);
	}
	
	if(GetWorld()->SweepMultiByChannel(results, start, end, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(SphereTraceRadius), collisionQueryParams))
	{
		APawn* swordHolder = GetInstigator();
		
		AController* instigator = nullptr;
		if(swordHolder)
		{
			instigator = swordHolder->GetInstigatorController();
		}

		for(FHitResult hitResult : results)
		{
			if(hitResult.GetActor())
			{
				FDamageEvent event;
				IgnoredActors.Add(hitResult.GetActor());
				
				hitResult.GetActor()->TakeDamage(Damage, event, instigator, this);
			}
		}
	}
}

void ASword::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(bIsDamageEnabled)
	{
		SweepAndApplyDamage();
	}
}


