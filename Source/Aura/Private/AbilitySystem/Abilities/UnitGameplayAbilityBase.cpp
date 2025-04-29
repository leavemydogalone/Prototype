// Copyright Scitcat Games


#include "AbilitySystem/Abilities/UnitGameplayAbilityBase.h"

#include "Game/TurnSystemInterface.h"
#include "Game/AuraTurnPhase.h"
#include "GameplayEffectTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/UnitAbilityPreviewContext.h"
#include "AbilitySystemBlueprintLibrary.h"
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

void UUnitGameplayAbilityBase::StartAbilityPreview()
{
	const FAuraGameplayTags& TagsManager = FAuraGameplayTags::Get();

	UUnitAbilityPreviewContext* UnitAbilityPreviewContext = NewObject<UUnitAbilityPreviewContext>(this);
	UnitAbilityPreviewContext->AbilityTag = AbilityTags.First();
	UnitAbilityPreviewContext->Unit = GetAvatarActorFromActorInfo();
	UnitAbilityPreviewContext->AbilityRange = AbilityRange;
	UnitAbilityPreviewContext->AbilitySize = AbilitySize;

	FGameplayEventData EventData;
	EventData.Instigator = GetAvatarActorFromActorInfo();
	EventData.OptionalObject = UnitAbilityPreviewContext;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(const_cast<AActor*>(CurrentEventData.Instigator.Get()), TagsManager.Event_Player_ShowAbilityPreview, EventData);
	
	//GetUnitInterface()->ShowAbilityPreview(AbilityTag, TargetLocation);
	//Get 
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
		if (GetAvatarActorFromActorInfo()->Implements<UUnitInterface>())
		{
			UnitInterface.SetObject(GetAvatarActorFromActorInfo());
			UnitInterface.SetInterface(Cast<IUnitInterface>(GetAvatarActorFromActorInfo()));
		}
	}

	return UnitInterface;
}
