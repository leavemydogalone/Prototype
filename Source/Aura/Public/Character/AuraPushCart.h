// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Game/AuraTurnPhase.h"
#include "AuraPushCart.generated.h"

class UBehaviorTree;
class AAuraAIController;

UCLASS()
class AURA_API AAuraPushCart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAuraPushCart();

	virtual void PossessedBy(AController* NewController) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	float DetectionRadius = 600.0f;

private:
	void HandleTurnPhaseChange(EAuraTurnPhase NewTurnPhase);

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
};
