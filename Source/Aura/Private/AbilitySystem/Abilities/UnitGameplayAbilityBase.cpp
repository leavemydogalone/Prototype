// Copyright Scitcat Games


#include "AbilitySystem/Abilities/UnitGameplayAbilityBase.h"

#include "Game/TurnSystemInterface.h"
#include "Game/AuraTurnPhase.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/GameState.h"

TScriptInterface<ITurnSystemInterface> UUnitGameplayAbilityBase::GetTurnSystemInterface()
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

void UUnitGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	UnbindFromTurnPhaseDelegate();
}

void UUnitGameplayAbilityBase::HandlePhaseEnumFromDelegate(EAuraTurnPhase TurnPhase)
{
	const FGameplayTag& TurnTag = GetTurnSystemInterface()->GetGameplayTagForTurnPhase(TurnPhase);
	OnTurnPhaseChangeReceived_BP(TurnTag);
}

void UUnitGameplayAbilityBase::BindToTurnPhaseDelegate()
{
	GetTurnSystemInterface()->GetOnTurnPhaseChangeDelegate().AddUObject(this, &UUnitGameplayAbilityBase::HandlePhaseEnumFromDelegate);
}
void UUnitGameplayAbilityBase::UnbindFromTurnPhaseDelegate()
{
	GetTurnSystemInterface()->GetOnTurnPhaseChangeDelegate().RemoveAll(this);
}
