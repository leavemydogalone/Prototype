// Copyright Scitcat Games


#include "Character/AuraPushCart.h"
#include "Game/TurnSystemInterface.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/AuraTurnPhase.h"

// Sets default values
AAuraPushCart::AAuraPushCart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AAuraPushCart::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	AuraAIController = Cast<AAuraAIController>(NewController);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	//AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	AuraAIController->GetBlackboardComponent()->SetValueAsFloat(FName("DetectionRadius"), DetectionRadius);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("PushIsLocked"), true);
}

// Called when the game starts or when spawned
void AAuraPushCart::BeginPlay()
{
	Super::BeginPlay();
	
	if (ITurnSystemInterface* TurnSystemInterface = GetWorld()->GetGameState<ITurnSystemInterface>())
	{
		FOnTurnPhaseChange& OnTurnPhaseChange = TurnSystemInterface->GetOnTurnPhaseChangeDelegate();
		OnTurnPhaseChange.AddUObject(this, &AAuraPushCart::HandleTurnPhaseChange);
	}
}

void AAuraPushCart::HandleTurnPhaseChange(EAuraTurnPhase NewTurnPhase)
{
	if (!HasAuthority()) return;
	if (NewTurnPhase != EAuraTurnPhase::Action_1 || NewTurnPhase != EAuraTurnPhase::Action_2)
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("PushIsLocked"), true);
		return;
	}
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("PushIsLocked"), false);
	//Probably am going to set a blackboard key
}


