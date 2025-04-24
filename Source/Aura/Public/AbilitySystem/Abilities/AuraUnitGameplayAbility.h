// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Game/TurnSystemInterface.h"
#include "AuraUnitGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUnitGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable, Category = "Turn System")
	TScriptInterface <ITurnSystemInterface> GetTurnSystemInterface();

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(BlueprintReadOnly, Category = "Turn System")
	TScriptInterface<ITurnSystemInterface> TurnSystemInterface;

	UFUNCTION(BlueprintImplementableEvent, Category = "Turn System")
	FGameplayTag OnTurnPhaseChangeReceived_BP(FGameplayTag TurnPhaseTag);

	UFUNCTION()
	void HandlePhaseEnumFromDelegate(EAuraTurnPhase TurnPhase);
	
	UFUNCTION(BlueprintCallable, Category = "Turn System")
	void BindToTurnPhaseDelegate();

private:
	void UnbindFromTurnPhaseDelegate();

};
