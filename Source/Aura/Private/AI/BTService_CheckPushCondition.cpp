// Copyright Scitcat Games


#include "AI/BTService_CheckPushCondition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Interaction/TeamInterface.h"

UBTService_CheckPushCondition::UBTService_CheckPushCondition()
{
	Interval = 0.5f;
	bNotifyBecomeRelevant = true;
}

void UBTService_CheckPushCondition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* CartPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;

    if (!CartPawn) return;

    FVector Origin = CartPawn->GetActorLocation();
    TArray<AActor*> NearbyActors;
    UGameplayStatics::GetAllActorsOfClass(CartPawn->GetWorld(), APawn::StaticClass(), NearbyActors);

    int32 OffenseCount = 0;
    int32 DefenseCount = 0;

    for (AActor* Actor : NearbyActors)
    {
		if (!Actor || Actor == CartPawn) continue;
		if (FVector::Dist(Actor->GetActorLocation(), Origin) > DetectionRadius) continue;

		// Will definitely need to find a different way to handle team assignments (based on offense and defense)
		if (ITeamInterface::Execute_GetTeamID(Actor) == 0)
			OffenseCount++;
		else if (ITeamInterface::Execute_GetTeamID(Actor) == 1)
			DefenseCount++;
	}

    bool bShouldPush = (OffenseCount > DefenseCount);
	int32 OffenseVsDefenseDifference = OffenseCount - DefenseCount;

	/*UBTFunctionLibrary::SetBlackboardValueAsInt(this, PushSpeedMultiplier, OffenseVsDefenseDifference);
	UBTFunctionLibrary::SetBlackboardValueAsBool(this, bShouldPushKey, bShouldPush);*/

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(bShouldPushKey.SelectedKeyName, bShouldPush);
}
