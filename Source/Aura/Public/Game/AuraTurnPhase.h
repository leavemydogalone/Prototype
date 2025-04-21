#pragma once

#include "CoreMinimal.h"
#include "AuraTurnPhase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAuraTurnPhase : uint8
{
    Planning UMETA(DisplayName = "Planning"),
    ActionPhase1 UMETA(DisplayName = "Action Phase 1"),
    ActionPhase2 UMETA(DisplayName = "Action Phase 2"),
	EndOfTurn UMETA(DisplayName = "End Of Turn"),
};
