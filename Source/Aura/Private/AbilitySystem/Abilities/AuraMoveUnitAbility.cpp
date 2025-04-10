// Copyright Druid Mechanics

#include "AbilitySystem/Abilities/AuraMoveUnitAbility.h"
#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

void UAuraMoveUnitAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraMoveUnitAbility::MoveToLocation(const FVector& TargetLocation)
{


}
