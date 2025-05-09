// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Game/TurnSystemInterface.h"
#include "Interaction/UnitInterface.h"
#include "Interaction/PlayerInterface.h"
#include "UnitGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UUnitGameplayAbilityBase : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
public:
	UUnitGameplayAbilityBase();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float AbilityRange = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float AbilitySize = 1.f;

	UPROPERTY(BlueprintReadOnly)
	FGameplayEventData ConfirmedEventData;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(BlueprintReadOnly, Category = "Turn System")
	TScriptInterface<ITurnSystemInterface> TurnSystemInterface;

	UFUNCTION(BlueprintImplementableEvent, Category = "Turn System")
	void ActivateUnitAbility();

	UFUNCTION()
	void HandlePhaseEnumFromDelegate(EAuraTurnPhase TurnPhase);

	UFUNCTION()
	void ShowAbilityPreview();

	UFUNCTION()
	void HideAbilityPreview();

private:

	TScriptInterface<ITurnSystemInterface> GetTurnSystemInterface();

	void BindToTurnPhaseDelegate();
	void UnbindFromTurnPhaseDelegate();

	TScriptInterface<IUnitInterface> GetUnitInterface();
	TScriptInterface<IPlayerInterface> GetPlayerInterface();

	UPROPERTY()
	TScriptInterface<IUnitInterface> UnitInterface;
	UPROPERTY()
	TScriptInterface<IPlayerInterface> PlayerInterface;

	UFUNCTION()
	void WaitForCancelTag();
	UFUNCTION()
	void OnCancelTagAdded(FGameplayEventData Data);

	UFUNCTION()
	void WaitForConfirmTag();
	UFUNCTION()
	void OnConfirmTagAdded(FGameplayEventData Data);

	UPROPERTY()
	bool bIsAbilityConfirmed = false; 

};
