// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CooldownGameplayAbility.h"
#include "SwordSwingAbility.generated.h"

class ACapsuleTraceTargetActor;
class APlayerShip;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class USwordSwingAbility : public UCooldownGameplayAbility
{
	GENERATED_BODY()
public:
	USwordSwingAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> SwordSwingMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ACapsuleTraceTargetActor> CapsuleTargetClass;
	
	UFUNCTION(BlueprintCallable)
	void AddMotionWarpingTarget(FName warpTargetName, AActor* warpActor, APlayerShip* playerShip);
private:
	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void TargetDataReceived( const FGameplayAbilityTargetDataHandle& data);

};
