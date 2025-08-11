// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/UnitGameplayAbilityBase.h"
#include "AuraAbilityTypes.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "UnitDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UUnitDamageGameplayAbility : public UUnitGameplayAbilityBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr,
		FVector InRadialDamageOrigin = FVector::ZeroVector,
		bool bOverrideKnockbackDirection = false,
		FVector KnockbackDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulse = false,
		FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f) const;


	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "TurnBased")
	bool bHasTurnBasedEffect = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TurnBased", meta = (EditCondition = "bHasTurnBasedEffect", EditConditionHides))
	const TSubclassOf<UGameplayEffect> TurnBasedEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffChance = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDamage = 0.f;

	// This will be game round length in seconds
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffFrequency = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDuration = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackForceMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackChance = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageOuterRadius = 0.f;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};
