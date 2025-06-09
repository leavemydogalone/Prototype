// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "TurnBasedAbilitySystemBPLibrary.generated.h"

struct FCustomContextData_TurnBaseEffect;
class UTurnBasedGameplayEffect;
class UTurnBasedGameplayEffectComponent;
class AAuraGameStateBase;
/**
 * 
 */
UCLASS()
class AURA_API UTurnBasedAbilitySystemBPLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Turn System", meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject"))
	static AAuraGameStateBase* GetTurnSystem(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Turn System", meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject"))
	static FCustomContextData_TurnBaseEffect GetTurnBasedEffectInstanceDataFromActiveEffectHandle(const FActiveGameplayEffectHandle& Handle);

	UFUNCTION(BlueprintCallable, Category = "Turn System", meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject"))
	static FCustomContextData_TurnBaseEffect GetTurnBasedEffectInstanceDataFromSpec(const FGameplayEffectSpec& Spec);

	UFUNCTION(BlueprintCallable, Category = "Turn System", meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject"))
	static void ApplyTurnBasedGameEffect(UAbilitySystemComponent* ASC, const FActiveGameplayEffect& ActiveGE, const TSubclassOf<UGameplayEffect>& GameEffectToApply, const bool bCopyContext = true);

	UFUNCTION(BlueprintCallable, Category = "Turn System", meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject"))
	static const UTurnBasedGameplayEffectComponent* ExtractTurnBasedEffectComponentFromEffectHandle(FActiveGameplayEffectHandle ActiveHandle);

	UFUNCTION(BlueprintCallable, Category = "Turn System", meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject"))
	static const UTurnBasedGameplayEffectComponent* ExtractTurnBasedEffectComponentFromGE(const UTurnBasedGameplayEffect* TurnEffect);

	UFUNCTION(BlueprintCallable, Category = "Turn System", meta = (bIgnoreSelf = "true", WorldContext = "WorldContextObject"))
	static FCustomContextData_TurnBaseEffect GetUpdatedTurnDataBasedOnTurn(const UObject* WorldContextObject, UPARAM(ref) const FCustomContextData_TurnBaseEffect& TurnBaseData, int32 TurnOverride = -1);
};
