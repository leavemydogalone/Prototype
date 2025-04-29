// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "UnitAbilityPreviewContext.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class AURA_API UUnitAbilityPreviewContext : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	FGameplayTag AbilityTag;

	UPROPERTY()
	AActor* Unit = nullptr;

	UPROPERTY()
	int32 AbilitySize = 1;

	UPROPERTY()
	int32 AbilityRange = 0;

	UPROPERTY()
	FVector TargetLocation;
};
