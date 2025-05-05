// Copyright Scitcat Games


#include "Objectives/Goal.h"
#include "Actor/AuraBall.h"

// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>("OverlapBox");
	OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapBox->SetupAttachment(RootComponent);

	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AGoal::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();
	
}


void AGoal::OnOverlapBegin
(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (AAuraBall* AuraBall = Cast<AAuraBall>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("AuraBall has entered the goal!"));
	}

}



