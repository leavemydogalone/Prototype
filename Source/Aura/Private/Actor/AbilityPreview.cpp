// Copyright Scitcat Games


#include "Actor/AbilityPreview.h"
#include "Components/DecalComponent.h"

// Sets default values
AAbilityPreview::AAbilityPreview()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbiltyTargetDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	AbiltyTargetDecal->SetupAttachment(GetRootComponent());

	AbilityRangeDecal = CreateDefaultSubobject<UDecalComponent>("AbilityRangeDecal");
	AbilityRangeDecal->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AAbilityPreview::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbilityPreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

