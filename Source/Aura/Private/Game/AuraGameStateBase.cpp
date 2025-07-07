// Copyright Druid Mechanics


#include "Game/AuraGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Game/AuraTurnPhase.h"
#include "AuraGameplayTags.h"
#include "Net/Core/PushModel/PushModel.h"

AAuraGameStateBase::AAuraGameStateBase()
{
	SetReplicates(true);
    const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	TurnPhaseToGameplayTagMap.Add(EAuraTurnPhase::Planning, GameplayTags.Phase_Planning);
	TurnPhaseToGameplayTagMap.Add(EAuraTurnPhase::Action_1, GameplayTags.Phase_Action_1);
	TurnPhaseToGameplayTagMap.Add(EAuraTurnPhase::Action_2, GameplayTags.Phase_Action_2);

}

void AAuraGameStateBase::AdvanceTurnPhase()
{
	Server_AdvanceTurnPhase();

}

FGameplayTag AAuraGameStateBase::GetGameplayTagForTurnPhase(EAuraTurnPhase& TurnPhase) const
{
	return TurnPhaseToGameplayTagMap.Contains(TurnPhase) ? TurnPhaseToGameplayTagMap[TurnPhase] : FGameplayTag();
}

void AAuraGameStateBase::StartRound()
{
}

void AAuraGameStateBase::StartRoundTime()
{
}

void AAuraGameStateBase::AdvanceCurrentRound()
{
	CurrentRound++;
	MARK_PROPERTY_DIRTY_FROM_NAME(AAuraGameStateBase, CurrentRound, this);
    OnRep_CurrentRound();

}

int32 AAuraGameStateBase::GetNumberOfRounds()
{
    return NumberOfRounds;
}


void AAuraGameStateBase::Server_AdvanceTurnPhase_Implementation()
{
    switch (CurrentTurnPhase)
    {
    case EAuraTurnPhase::Planning:
        CurrentTurnPhase = EAuraTurnPhase::Action_1;
        HandleActionTurnCountChange();
        break;
    case EAuraTurnPhase::Action_1:
        CurrentTurnPhase = EAuraTurnPhase::Action_2;
        HandleActionTurnCountChange();
        break;
    case EAuraTurnPhase::Action_2:
        CurrentTurnPhase = EAuraTurnPhase::Cleanup;
        HandleCleanUp();
        break;
    case EAuraTurnPhase::Cleanup:
        CurrentTurnPhase = EAuraTurnPhase::Planning;
        break;
    default:
        CurrentTurnPhase = EAuraTurnPhase::Planning;
        break;
    }
	MARK_PROPERTY_DIRTY_FROM_NAME(AAuraGameStateBase, CurrentTurnPhase, this);

	OnRep_CurrentTurnPhase();
}

void AAuraGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	DOREPLIFETIME_WITH_PARAMS_FAST(AAuraGameStateBase, CurrentTurnPhase, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(AAuraGameStateBase, CurrentActionTurnCount, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(AAuraGameStateBase, CurrentRound, Params);
}

void AAuraGameStateBase::HandleActionTurnCountChange()
{
    CurrentActionTurnCount = CurrentActionTurnCount + 1;
    OnRep_CurrentActionTurnCount();
    MARK_PROPERTY_DIRTY_FROM_NAME(AAuraGameStateBase, CurrentActionTurnCount, this);

}

void AAuraGameStateBase::HandleCleanUp()
{
    if (CurrentRound < NumberOfRounds) AdvanceCurrentRound();
}
