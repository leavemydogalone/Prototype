// Copyright Druid Mechanics


#include "Game/AuraGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Game/AuraTurnPhase.h"
#include "Net/Core/PushModel/PushModel.h"

AAuraGameStateBase::AAuraGameStateBase()
{
	SetReplicates(true);
}

void AAuraGameStateBase::AdvanceTurnPhase()
{
	Server_AdvanceTurnPhase();

}

void AAuraGameStateBase::Server_AdvanceTurnPhase_Implementation()
{
	CurrentTurnPhase = EAuraTurnPhase::ActionPhase1;
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
}
