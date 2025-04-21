#pragma once

#include "CoreMinimal.h"
#include "AuraTurnPhase.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAuraTurnPhase : uint8
{
    Phase1 UMETA(DisplayName = "Phase 1"),
    Phase2 UMETA(DisplayName = "Phase 2"),
    Phase3 UMETA(DisplayName = "Phase 3"),
    Phase4 UMETA(DisplayName = "Phase 4"),
    Phase5 UMETA(DisplayName = "Phase 5")
};
