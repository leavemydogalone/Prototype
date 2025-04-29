// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystem/Data/UnitAbilityPreviewContext.h"
#include "ShowUnitAbilityPreview.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UShowUnitAbilityPreview : public UAbilityTask
{
	GENERATED_BODY()
	
    UPROPERTY()
    UUnitAbilityPreviewContext* PreviewContext;

    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Show Ability Preview", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility"))
    static UShowUnitAbilityPreview* ShowUnitAbilityPreview(UGameplayAbility* OwningAbility, UUnitAbilityPreviewContext* InContext);

    virtual void Activate() override;
};
