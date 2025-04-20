// Copyright Druid Mechanics


#include "Game/AuraGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

AAuraGameStateBase::AAuraGameStateBase()
{
	SetReplicates(true);
}

void AAuraGameStateBase::IncrementTurn()
{
	Server_IncrementTurn();

}

void AAuraGameStateBase::Server_IncrementTurn_Implementation()
{
	CurrentTurn++;
	MARK_PROPERTY_DIRTY_FROM_NAME(AAuraGameStateBase, CurrentTurn, this);
	OnRep_CurrentTurn();
}

void AAuraGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams Params{};
	Params.bIsPushBased = true;
	Params.Condition = COND_None;

	DOREPLIFETIME_WITH_PARAMS_FAST(AAuraGameStateBase, CurrentTurn, Params);
}
