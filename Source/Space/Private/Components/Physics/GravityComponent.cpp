// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Physics/GravityComponent.h"
#include "Components/SceneComponent.h"
#include "Components/MeshComponent.h"
#include "Utility/PriorityQueue.h"
#include "GravityVolume.h"

// Sets default values for this component's properties
UGravityComponent::UGravityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

}


// Called when the game starts
void UGravityComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UGravityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	ApplyGravity();
}

void UGravityComponent::EnteredGravityVolume(UGravityVolume* volume)
{
	if(!volume)return;
	GravityZonesQueue.Push(volume, volume->GetVolumePriority());
}

void UGravityComponent::ExitedGravityVolume(UGravityVolume* volume)
{
	if(!volume)return;


	GravityZonesQueue.Remove(volume);
}

void UGravityComponent::ApplyGravity() const
{

	USceneComponent* rootComp = GetOwner()->GetRootComponent();
	if(!rootComp ) return;

	UMeshComponent* shipMesh = Cast<UMeshComponent>(rootComp);
	if(!shipMesh)return;

	shipMesh->AddForce(GetGravityDirection() * shipMesh->GetMass(), FName("None"), true);
}


FVector UGravityComponent::GetGravityDirection() const
{
	if(GravityZonesQueue.IsEmpty() || GravityZonesQueue.Top() == nullptr )	return DefaultGravityDirection;

	return GravityZonesQueue.Top()->GetGravityDirection(GetOwner()->GetActorLocation());
}


