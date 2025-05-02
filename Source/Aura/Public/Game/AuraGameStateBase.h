// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TurnSystemInterface.h"
#include "AuraTurnPhase.h"
#include "GameManagerInterface.h"
#include "AuraGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameStateBase : public AGameStateBase, public ITurnSystemInterface, public IGameManagerInterface
{
	GENERATED_BODY()
	
public:
	AAuraGameStateBase();

	// Begin ITurnSystemInterface
	virtual FOnTurnPhaseChange& GetOnTurnPhaseChangeDelegate() override { return OnTurnPhaseChange; }

	virtual EAuraTurnPhase GetCurrentTurnPhase() const override { return CurrentTurnPhase; }

	virtual void AdvanceTurnPhase() override;

	UFUNCTION(Server, Reliable)
	virtual void Server_AdvanceTurnPhase();

	virtual FGameplayTag GetGameplayTagForTurnPhase(EAuraTurnPhase& TurnPhase) const override;

	// End ITurnSystemInterface

	// Begin IGameManagerInterface
	virtual int8 GetRemainingGameTime() override;
	virtual int8 GetRemainingQuarterTime() override;
	virtual int8 GetRemainingRoundTime() override;
	virtual int8 GetQuarter() override;



protected:

	FOnTurnPhaseChange OnTurnPhaseChange;

	UPROPERTY(BlueprintAssignable, Category = "Turn System")
	FOnTurnPhaseChangeAssignable OnTurnPhaseChangeDelegate;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentTurnPhase)
	EAuraTurnPhase CurrentTurnPhase = EAuraTurnPhase::Planning;

	UFUNCTION()
	void OnRep_CurrentTurnPhase() const
	{
		OnTurnPhaseChange.Broadcast(CurrentTurnPhase);
		OnTurnPhaseChangeDelegate.Broadcast(CurrentTurnPhase);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	TMap<EAuraTurnPhase, FGameplayTag> TurnPhaseToGameplayTagMap;

	UPROPERTY()
	int8 StartingGameTime = 60 * 9;
	UPROPERTY()
	int8 RemainingGameTime = StartingGameTime;
	UPROPERTY()
	int8 StartingQuarterTime = StartingGameTime / 4;
	UPROPERTY()
	int8 RemainingQuarterTime = StartingQuarterTime;
	UPROPERTY()
	int8 CurrentQuarter = 1;
	UPROPERTY()
	int8 StartingRoundTime = 10;
	UPROPERTY()
	int8 RemainingRoundTime = StartingRoundTime;
};
