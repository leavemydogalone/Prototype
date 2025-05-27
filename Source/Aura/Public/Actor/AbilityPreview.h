// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityPreview.generated.h"

class USplineComponent;
class USplineMeshComponent;

UCLASS()
class AURA_API AAbilityPreview : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityPreview();

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//TObjectPtr<UDecalComponent> AbiltyTargetDecal;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//TObjectPtr<UDecalComponent> AbilityRangeDecal;

	UFUNCTION()
	void UpdateSpline(const TArray<FVector>& Points);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RebuildSplineMeshSegments();

	UPROPERTY()
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY()
	TArray<TObjectPtr<USplineMeshComponent>> SplineMeshes;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> SplineMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInterface> PreviewMaterial;

	TArray<FVector> CurrentPoints;
};
