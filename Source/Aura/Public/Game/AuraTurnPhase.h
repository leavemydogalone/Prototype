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
    Action_1 UMETA(DisplayName = "Action 1"),
    Action_2 UMETA(DisplayName = "Action 2"),
	Cleanup UMETA(DisplayName = "Cleanup"),
};
