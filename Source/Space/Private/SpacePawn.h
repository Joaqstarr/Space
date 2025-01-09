// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Pawn.h"
#include "SpacePawn.generated.h"
class UGameplayEffect;
class UGameplayAbility;
class UShipAbilitySystemComponent;
UCLASS()
class ASpacePawn : public APawn, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpacePawn();
	UFUNCTION(BlueprintCallable)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
protected:
	
	/*
	 *	GAMEPLAY ABILITY SYSTEM
	 */
	virtual void BeginPlay() override;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UShipAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UFUNCTION(Client, Reliable)
	void PosessedClient(AController* NewController);
	UFUNCTION(BlueprintImplementableEvent)
	void PosessedClientBP(AController* NewController);
	UFUNCTION(Client, Reliable)
	void UnPosessedClient(AController* OldController);
	UFUNCTION(BlueprintImplementableEvent)
	void UnPosessedClientBP(AController* OldController);
private:
	void InitDefaultAttributes() const;
	void GiveDefaultAbilities();

	UPROPERTY()
	AController* CurrentController;

};
