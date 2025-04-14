// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraUnitMoveAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUnitMoveAbility : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
	// We want to capture the unit's intelligence attribute to determine the distance moved
	// We want to enable overlap damage volume on moving unit
	// 
	// We want to move the unit the distance of the intelligence attribute to the target location
	// 

	//Look at Arcane shards for example
	// Player controller, create a show ability preview function
};
