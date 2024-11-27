// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "AngleLimiterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UAngleLimiterComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UAngleLimiterComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Separate angular limits for each axis
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle Limits")
	float MinPitchLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle Limits")
	float MaxPitchLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle Limits")
	float MinYawLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle Limits")
	float MaxYawLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle Limits")
	float MinRollLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle Limits")
	float MaxRollLimit;

	// Target child component to constrain
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle Limits")
	USceneComponent* TargetComponent;

	// Whether to use relative or world space for rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Angle Limits")
	bool bUseRelativeSpace = true;

#if WITH_EDITOR
	virtual void OnRegister() override;
	void DebugDrawComponent() const;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	void ApplyAngleLimits(float DeltaTime);
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebugLines = true;
	
		
};
