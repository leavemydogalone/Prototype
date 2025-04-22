// Copyright Scitcat Games


#include "Actor/AuraBall.h"
#include "Game/TurnSystemInterface.h"
#include "Game/AuraTurnPhase.h"

AAuraBall::AAuraBall()
{
	PrimaryActorTick.bCanEverTick = true;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetupAttachment(RootComponent);
}

void AAuraBall::BeginPlay()
{
	Super::BeginPlay();
	
	if (ITurnSystemInterface* TurnSystemInterface = GetWorld()->GetGameState<ITurnSystemInterface>())
	{
		FOnTurnPhaseChange& OnTurnPhaseChange = TurnSystemInterface->GetOnTurnPhaseChangeDelegate();
		OnTurnPhaseChange.AddUObject(this, &AAuraBall::HandleTurnPhaseChange);
	}
}

void AAuraBall::HandleTurnPhaseChange(EAuraTurnPhase NewTurnPhase)
{
	// Handle turn phase change logic here
	UE_LOG(LogTemp, Warning, TEXT("Turn phase changed on ball!"));

	switch (NewTurnPhase)
	{
	case EAuraTurnPhase::Planning:
		FreezeBall();
		break;
	case EAuraTurnPhase::ActionPhase1:
		UnFreezeBall();
		break;
	default:
		break;
	}
}

void AAuraBall::FreezeBall()
{
	if (BallMesh->IsSimulatingPhysics())
	{
		StoredLinearVelocity = BallMesh->GetPhysicsLinearVelocity();
		StoredAngularVelocity = BallMesh->GetPhysicsAngularVelocityInDegrees();
		BallMesh->SetSimulatePhysics(false);
	}
}

void AAuraBall::UnFreezeBall()
{
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetPhysicsLinearVelocity(StoredLinearVelocity);
	BallMesh->SetPhysicsAngularVelocityInDegrees(StoredAngularVelocity);
}


