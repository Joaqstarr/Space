// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Components/Player/PawnMotionWarpingComponent.h"
#include "PawnMotionWarpingState.generated.h"

class UPawnMotionWarpingComponent;

UCLASS(meta = (DisplayName = "Pawn Motion Warping"), Blueprintable)
class UPawnMotionWarpingState : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, float totalDuration, const FAnimNotifyEventReference& eventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, float frameDeltaTime, const FAnimNotifyEventReference& eventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& eventReference) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAlphaBlendOption AddTranslationEasingFunc;



	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMotionWarpingParams MotionWarpingParams;
private:
	TWeakObjectPtr<UPawnMotionWarpingComponent> PawnMotionWarpingComponent;

	float Duration = 1;
	float CurrentTime = 0;

	FAlphaBlend Blender;
};
