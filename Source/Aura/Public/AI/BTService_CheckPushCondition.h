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

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FBlackboardKeySelector DetectionRadius;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FBlackboardKeySelector ShouldPush;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FBlackboardKeySelector PushSpeedMultiplier;
};
