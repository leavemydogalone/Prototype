// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AuraTurnPhase.h"
#include "GameplayTagContainer.h"
#include "TurnSystemInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTurnPhaseChange, EAuraTurnPhase NewTurnPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnPhaseChangeAssignable, EAuraTurnPhase, NewTurnPhase);

// This class does not need to be modified. This is the class that will be inherited to implement this interface.

UINTERFACE(MinimalAPI, NotBlueprintable)
class UTurnSystemInterface : public UInterface
{
GENERATED_BODY()
};

/**
* Manage the current turn and broadcast the "OnTurnChanged" event
*/
class AURA_API ITurnSystemInterface
{
GENERATED_BODY()

public:
virtual FOnTurnPhaseChange& GetOnTurnPhaseChangeDelegate() = 0;

UFUNCTION(BlueprintCallable, Category = "Turn System")
virtual EAuraTurnPhase GetCurrentTurnPhase() const { return EAuraTurnPhase::Planning; };

UFUNCTION(BlueprintCallable, Category = "Turn System")
virtual void AdvanceTurnPhase() {};

// New method to relate turn phases with gameplay tags
UFUNCTION(BlueprintCallable, Category = "Turn System")
virtual FGameplayTag GetGameplayTagForTurnPhase(EAuraTurnPhase& TurnPhase) const = 0;
};
