// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Game/TurnSystemInterface.h"
#include "UnitGameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UUnitGameplayAbilityBase : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable, Category = "Turn System")
	TScriptInterface <ITurnSystemInterface> GetTurnSystemInterface();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(BlueprintReadOnly, Category = "Turn System")
	TScriptInterface<ITurnSystemInterface> TurnSystemInterface;

	UFUNCTION(BlueprintImplementableEvent, Category = "Turn System")
	void ActivateUnitAbility();

	UFUNCTION()
	void HandlePhaseEnumFromDelegate(EAuraTurnPhase TurnPhase);

	UFUNCTION(BlueprintCallable, Category = "Turn System")
	void BindToTurnPhaseDelegate();

private:
	void UnbindFromTurnPhaseDelegate();
};
