// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckPushCondition.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UBTService_CheckPushCondition : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckPushCondition();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditDefaultsOnly, Category = "Detection")
    float DetectionRadius = 600.0f;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FBlackboardKeySelector bShouldPushKey;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FBlackboardKeySelector PushSpeedMultiplier;
};
