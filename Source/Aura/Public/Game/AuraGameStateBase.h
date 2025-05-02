// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TurnSystemInterface.h"
#include "AuraTurnPhase.h"
#include "RoundManagerInterface.h"
#include "AuraGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameStateBase : public AGameStateBase, public ITurnSystemInterface, public IRoundManagerInterface
{
	GENERATED_BODY()
	
public:
	AAuraGameStateBase();

	// Begin ITurnSystemInterface
	virtual FOnTurnPhaseChange& GetOnTurnPhaseChangeDelegate() override { return OnTurnPhaseChange; }
	virtual EAuraTurnPhase GetCurrentTurnPhase() const override { return CurrentTurnPhase; }
	virtual void AdvanceTurnPhase() override;
	virtual FGameplayTag GetGameplayTagForTurnPhase(EAuraTurnPhase& TurnPhase) const override;

	UFUNCTION(Server, Reliable)
	virtual void Server_AdvanceTurnPhase();
	// End ITurnSystemInterface


	 // Begin IGameManagerInterface
	 virtual FOnRoundChange& GetOnRoundChangeDelegate() override { return OnRoundChange; };
	 virtual void StartRound() override;
	 virtual void StartRoundTime() override;
	 //virtual const int32 GetCurrentRoundTime() override { return CurrentRoundTime; }
	 virtual int32 GetCurrentRound() const override { return CurrentRound; }
	 virtual void AdvanceCurrentRound() override;
	 // End IGameManagerInterface

protected:

	// Round System
	FOnRoundChange OnRoundChange;

	UPROPERTY(BlueprintAssignable, Category = "Round Manager")
	FOnRoundChangeAssignable OnRoundChangeDelegate;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentRound)
	int32 CurrentRound = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Round Manager")
	int32 StartingRoundTime;

	UPROPERTY(EditDefaultsOnly, Category = "Round Manager")
	int32 NumberOfRounds;

	UFUNCTION()
	void OnRep_CurrentRound() const
	{
		OnRoundChange.Broadcast(CurrentRound);
		OnRoundChangeDelegate.Broadcast(CurrentRound);
	}

	//UFUNCTION()
	//void OnRep_CurrentTurn() const
	//{
	//	OnTurnPhaseChange.Broadcast(CurrentTurnPhase);
	//	OnTurnPhaseChangeDelegate.Broadcast(CurrentTurnPhase);
	//}


	// Turn System
	FOnTurnPhaseChange OnTurnPhaseChange;

	UPROPERTY(BlueprintAssignable, Category = "Turn System")
	FOnTurnPhaseChangeAssignable OnTurnPhaseChangeDelegate;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentTurnPhase)
	EAuraTurnPhase CurrentTurnPhase = EAuraTurnPhase::Planning;

	//UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentTurnTime)
	//int32 CurrentTurnTime = StartingTurnTime;

	//UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentTurn)
	//int32 CurrentTurn = 0;

	UFUNCTION()
	void OnRep_CurrentTurnPhase() const
	{
		OnTurnPhaseChange.Broadcast(CurrentTurnPhase);
		OnTurnPhaseChangeDelegate.Broadcast(CurrentTurnPhase);
	}

	/*UFUCNTION()
		OnRep_CurrentTurnTime() const
	{
		OnTurnPhaseChange.Broadcast(CurrentTurnPhase);
		OnTurnPhaseChangeDelegate.Broadcast(CurrentTurnPhase);
	}*/

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;



private:

	TMap<EAuraTurnPhase, FGameplayTag> TurnPhaseToGameplayTagMap;

	UPROPERTY()
	int32 StartingTurnTime = 10;
};
