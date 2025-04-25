// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h" // Include the header that defines FGameplayTag
#include "UnitInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UUnitInterface : public UInterface
{
	GENERATED_BODY()
};

struct FGameplayTag;

/**
 * 
 */
class AURA_API IUnitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Unit Interface")
	void ShowAbilityPreview(FGameplayTag AbilityTag, FVector TargetLocation);
};
