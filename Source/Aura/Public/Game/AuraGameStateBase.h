// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TurnSystemInterface.h"
#include "AuraTurnPhase.h"
#include "AuraGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraGameStateBase : public AGameStateBase, public ITurnSystemInterface
{
	GENERATED_BODY()
	
public:
	AAuraGameStateBase();

	// Begin ITurnSystemInterface
	virtual FOnTurnPhaseChange& GetOnTurnPhaseChangeDelegate() override { return OnTurnPhaseChange; }

	virtual EAuraTurnPhase GetCurrentTurnPhase() const override { return CurrentTurnPhase; }

	virtual void AdvanceTurnPhase() override;
	// End ITurnSystemInterface

	UFUNCTION(Server, Reliable)
	virtual void Server_AdvanceTurnPhase();

	virtual FGameplayTag GetGameplayTagForTurnPhase(EAuraTurnPhase& TurnPhase) const override;

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
};
