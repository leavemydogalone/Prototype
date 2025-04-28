// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityPreview.generated.h"

UCLASS()
class AURA_API AAbilityPreview : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityPreview();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> AbiltyTargetDecal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> AbilityRangeDecal;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
