// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Player/PawnMotionWarping/PawnMotionWarpingState.h"

#include "Components/Player/PawnMotionWarpingComponent.h"

void UPawnMotionWarpingState::NotifyBegin(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation,
	float totalDuration, const FAnimNotifyEventReference& eventReference)
{
	Super::NotifyBegin(meshComp, animation, totalDuration, eventReference);
	//if (!meshComp || !meshComp->GetWorld() || !GetWorld()->IsGameWorld()) return;

	if(PawnMotionWarpingComponent == nullptr)
	{
		AActor* owner = meshComp->GetOwner();
		if(owner != nullptr)
		{
			PawnMotionWarpingComponent = owner->GetComponentByClass<UPawnMotionWarpingComponent>();


		}
	}
	Duration = totalDuration;
	CurrentTime = 0;
	
	if(PawnMotionWarpingComponent != nullptr)
	{
		PawnMotionWarpingComponent->StartWarping(MotionWarpingParams);
	}
}

void UPawnMotionWarpingState::NotifyTick(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation,
	float frameDeltaTime, const FAnimNotifyEventReference& eventReference)
{

	Super::NotifyTick(meshComp, animation, frameDeltaTime, eventReference);
	//if (!meshComp || !meshComp->GetWorld() || !GetWorld()->IsGameWorld()) return;

	if(PawnMotionWarpingComponent == nullptr)return;

	CurrentTime += frameDeltaTime;
	
	if(Duration == 0)Duration = 0.01;

	float rawInterp = CurrentTime/Duration;

	float transformedAlpha = Blender.AlphaToBlendOption(rawInterp, AddTranslationEasingFunc);
	PawnMotionWarpingComponent->UpdateMotionWarping(transformedAlpha);
}

void UPawnMotionWarpingState::NotifyEnd(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation,
	const FAnimNotifyEventReference& eventReference)
{
	Super::NotifyEnd(meshComp, animation, eventReference);

//	if (!meshComp || !meshComp->GetWorld() || !GetWorld()->IsGameWorld()) return;

	if(PawnMotionWarpingComponent != nullptr)
	{
		PawnMotionWarpingComponent->StopWarping();
	}
}
