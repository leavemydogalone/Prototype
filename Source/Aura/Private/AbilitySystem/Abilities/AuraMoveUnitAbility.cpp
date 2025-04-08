// Copyright Druid Mechanics

#include "AbilitySystem/Abilities/AuraMoveUnitAbility.h"


void UAuraMoveUnitAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraMoveUnitAbility::MoveToLocation(const FVector& TargetLocation)
{
	//write debug message to screen of the target location
	//UE_LOG(LogTemp, Warning, TEXT("Target Location: %s"), *TargetLocation.ToString());
	GEngine->AddOnScreenDebugMessage(
		-1,                      // Key (-1 = auto-add)
		5.0f,                    // Time to display (in seconds)
		FColor::Green,           // Color
		TEXT("Target Location")  // The message
	);

	//if (const AActor* TargetActor = GetAvatarActorFromActorInfo())
	//{
	//	FAIMoveRequest MoveRequest;
	//	MoveRequest.SetGoalLocation(TargetLocation);
	//	MoveRequest.SetAcceptanceRadius(5.f);
	//	const FNavPathSharedPtr NavPath = UAuraAbilitySystemLibrary::GetNavPath(TargetActor, MoveRequest);
	//	if (NavPath.IsValid())
	//	{
	//		UAuraAbilitySystemLibrary::MoveToLocation(TargetActor, MoveRequest);
	//	}
	//}
}