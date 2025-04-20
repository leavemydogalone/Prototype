// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TurnSystemInterface.h"
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
	virtual FOnTurnChange& GetOnTurnChangeDelegate() override { return OnTurnChange; }

	virtual int32 GetCurrentTurn() const override { return CurrentTurn; }

	virtual void IncrementTurn() override;
	// End ITurnSystemInterface

	UFUNCTION(Server, Reliable)
	virtual void Server_IncrementTurn();

protected:

	FOnTurnChange OnTurnChange;

	UPROPERTY(BlueprintAssignable, Category = "Turn System")
	FOnTurnChangeAssignable OnTurnChangeDelegate;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentTurn)
	int32 CurrentTurn = 0;

	UFUNCTION()
	void OnRep_CurrentTurn() const
	{
		OnTurnChange.Broadcast(CurrentTurn);
		OnTurnChangeDelegate.Broadcast(CurrentTurn);
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
