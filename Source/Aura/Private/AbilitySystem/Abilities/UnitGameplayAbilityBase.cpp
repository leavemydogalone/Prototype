// Copyright Scitcat Games


#include "AbilitySystem/Abilities/UnitGameplayAbilityBase.h"

#include "Game/TurnSystemInterface.h"
#include "Game/AuraTurnPhase.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/GameState.h"


void UUnitGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	BindToTurnPhaseDelegate();
	//StartAbilityPreview();
	//Wait for confirmation event
}

void UUnitGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	UnbindFromTurnPhaseDelegate();
}


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

void UUnitGameplayAbilityBase::HandlePhaseEnumFromDelegate(EAuraTurnPhase TurnPhase)
{
	FGameplayTag TurnTag = GetTurnSystemInterface()->GetGameplayTagForTurnPhase(TurnPhase);
	FGameplayTagContainer& InstigatorTags = CurrentEventData.InstigatorTags;
	for (const FGameplayTag& Tag : InstigatorTags)
	{
		if (Tag.MatchesTag(TurnTag))
		{
			ActivateUnitAbility();
		}
	}
	
}

void UUnitGameplayAbilityBase::HandleAbilityPreview(FGameplayTag AbilityTag, FVector TargetLocation)
{
	GetUnitInterface()->ShowAbilityPreview(AbilityTag, TargetLocation);
}

void UUnitGameplayAbilityBase::BindToTurnPhaseDelegate()
{
	GetTurnSystemInterface()->GetOnTurnPhaseChangeDelegate().AddUObject(this, &UUnitGameplayAbilityBase::HandlePhaseEnumFromDelegate);
}
void UUnitGameplayAbilityBase::UnbindFromTurnPhaseDelegate()
{
	GetTurnSystemInterface()->GetOnTurnPhaseChangeDelegate().RemoveAll(this);
}

TScriptInterface<IUnitInterface> UUnitGameplayAbilityBase::GetUnitInterface()
{
	if (!UnitInterface.GetObject())
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		if (GameState && GameState->Implements<UUnitInterface>())
		{
			UnitInterface.SetObject(GameState);
			UnitInterface.SetInterface(Cast<IUnitInterface>(GameState));
		}
	}

	return UnitInterface;
}
