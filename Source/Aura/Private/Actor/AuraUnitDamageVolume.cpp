// Copyright Scitcat Games


#include "Actor/AuraUnitDamageVolume.h"

// Sets default values
AAuraUnitDamageVolume::AAuraUnitDamageVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAuraUnitDamageVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraUnitDamageVolume::OnHit()
{
}

void AAuraUnitDamageVolume::Destroyed()
{
}



