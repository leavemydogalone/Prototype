// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "OnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTickTaskDelegate, float, DeltaTime);

/**
 * Task for abilities that supply tick and its' delta time.
 */
UCLASS()
class AURA_API UOnTick : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FOnTickTaskDelegate OnTick;

public:

	UOnTick(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UOnTick* AbilityTaskOnTick(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName);

	virtual void Activate() override;
	virtual void TickTask(float DeltaTime) override;
};
