// Copyright Scitcat Games


#include "AbilitySystem/Abilities/UnitGameplayAbilityBase.h"
#include "AuraGameplayTags.h"
#include "Game/TurnSystemInterface.h"
#include "Game/AuraTurnPhase.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/UnitAbilityPreviewContext.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GameFramework/GameState.h"


void UUnitGameplayAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	BindToTurnPhaseDelegate();
	ShowAbilityPreview();
	WaitForCancelTag();
	//Wait for confirmation event
}

void UUnitGameplayAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	UnbindFromTurnPhaseDelegate();
	HideAbilityPreview();
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

void UUnitGameplayAbilityBase::ShowAbilityPreview()
{
	FUnitAbilityPreviewInfo UnitAbilityPreviewInfo;
	UnitAbilityPreviewInfo.AbilityTag = AbilityTags.First();
	UnitAbilityPreviewInfo.Unit = GetAvatarActorFromActorInfo();
	UnitAbilityPreviewInfo.AbilityRange = AbilityRange;
	UnitAbilityPreviewInfo.AbilitySize = AbilitySize;

	GetPlayerInterface()->ShowAbilityPreview(UnitAbilityPreviewInfo);
}

void UUnitGameplayAbilityBase::HideAbilityPreview()
{
	GetPlayerInterface()->HideAbilityPreview();
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

TScriptInterface<IPlayerInterface> UUnitGameplayAbilityBase::GetPlayerInterface()
{
	if (!PlayerInterface.GetObject())
	{
		TObjectPtr<const AActor> Instigator = CurrentEventData.Instigator;
		AActor* Player = const_cast<AActor*>(Instigator.Get());

		if (Player->Implements<UPlayerInterface>())
		{
			PlayerInterface.SetObject(Player);
			PlayerInterface.SetInterface(Cast<IPlayerInterface>(Player));
		}
	}

	return PlayerInterface;
}

void UUnitGameplayAbilityBase::WaitForCancelTag()
{
	FGameplayTag CancelTag = FAuraGameplayTags::Get().Event_Unit_Cancel;

	UAbilityTask_WaitGameplayEvent* WaitForCancelTagTask =
		UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, CancelTag);
	if (WaitForCancelTagTask)
	{
		WaitForCancelTagTask->EventReceived.AddDynamic(this, &UUnitGameplayAbilityBase::OnCancelTagAdded);
		WaitForCancelTagTask->ReadyForActivation();
	}
}

void UUnitGameplayAbilityBase::OnCancelTagAdded(FGameplayEventData Data)
{
	FGameplayTag ConfirmTag = FAuraGameplayTags::Get().Event_Unit_Confirm;
	if(GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(ConfirmTag)) return;
	HideAbilityPreview();
	CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
}
