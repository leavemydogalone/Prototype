// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage_V2.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UExecCalc_Damage_V2 : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecCalc_Damage_V2();

	void DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		const FGameplayEffectSpec& Spec,
		FAggregatorEvaluateParameters EvaluationParameters,
		const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const;

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
