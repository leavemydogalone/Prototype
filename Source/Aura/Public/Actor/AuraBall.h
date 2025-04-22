// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/AuraTurnPhase.h"
#include "AuraBall.generated.h"


UCLASS()
class AURA_API AAuraBall : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraBall();

protected:
	virtual void BeginPlay() override;

	void HandleTurnPhaseChange(EAuraTurnPhase NewTurnPhase);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball Components")
	TObjectPtr<UStaticMeshComponent> BallMesh;

private:

    FVector StoredLinearVelocity;  
    FVector StoredAngularVelocity;

	void FreezeBall();
	void UnFreezeBall();
};
