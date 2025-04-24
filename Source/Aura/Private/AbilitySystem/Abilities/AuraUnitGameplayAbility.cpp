// Copyright Scitcat Games


#include "AbilitySystem/Abilities/AuraUnitGameplayAbility.h"
#include "Game/TurnSystemInterface.h"
#include "Game/AuraTurnPhase.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/GameState.h"

TScriptInterface<ITurnSystemInterface> UAuraUnitGameplayAbility::GetTurnSystemInterface()
{
	if (!TurnSystemInterface.GetObject())
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		if (GameState && GameState->Implements<UTurnSystemInterface>()) 
		{
			TurnSystemInterface.SetObject(GameState);
			TurnSystemInterface.SetInterface(Cast<ITurnSystemInterface>(GameState));
		}
	}

	return TurnSystemInterface;
}

void UAuraUnitGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	UnbindFromTurnPhaseDelegate();
}

void UAuraUnitGameplayAbility::HandlePhaseEnumFromDelegate(EAuraTurnPhase TurnPhase)
{
	const FGameplayTag& TurnTag = GetTurnSystemInterface()->GetGameplayTagForTurnPhase(TurnPhase);
	OnTurnPhaseChangeReceived_BP(TurnTag);
}

void UAuraUnitGameplayAbility::BindToTurnPhaseDelegate()
{
	GetTurnSystemInterface()->GetOnTurnPhaseChangeDelegate().AddUObject(this, &UAuraUnitGameplayAbility::HandlePhaseEnumFromDelegate);
}
void UAuraUnitGameplayAbility::UnbindFromTurnPhaseDelegate()
{
	GetTurnSystemInterface()->GetOnTurnPhaseChangeDelegate().RemoveAll(this);
}


