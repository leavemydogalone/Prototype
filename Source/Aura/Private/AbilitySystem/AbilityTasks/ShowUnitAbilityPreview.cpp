// Copyright Scitcat Games

#include "AbilitySystem/AbilityTasks/ShowUnitAbilityPreview.h"
#include "AbilitySystem/Data/UnitAbilityPreviewContext.h"
#include "GameFramework/PlayerController.h"
#include "Player/AuraPlayerController.h"

UShowUnitAbilityPreview* UShowUnitAbilityPreview::ShowUnitAbilityPreview(UGameplayAbility* OwningAbility, UUnitAbilityPreviewContext* InContext)
{
    UShowUnitAbilityPreview* Task = NewAbilityTask<UShowUnitAbilityPreview>(OwningAbility);
    Task->PreviewContext = InContext;
    return Task;
}

void UShowUnitAbilityPreview::Activate()
{
    if (Ability && Ability->GetCurrentActorInfo()->IsLocallyControlled())
    {
        if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Ability->GetCurrentActorInfo()->PlayerController.Get()))
        {
            PC->ShowAbilityPreview(PreviewContext);
        }
    }
    EndTask();
}