// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystem/Data/AbilityInfo.h"
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
	void UpdateAbilityPreview(const FVector& TargetLocation);

	UFUNCTION()
	void UpdateSpline(const TArray<FVector>& Points);

	UFUNCTION()
	void SetUnitAbilityPreviewInfo(const FUnitAbilityPreviewInfo& InUnitAbilityPreviewInfo)
	{
		UnitAbilityPreviewInfo = InUnitAbilityPreviewInfo;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void RebuildSplineMeshSegments();
	void DrawDebugCircleAroundActor(AActor* TargetActor, float MaxRange, int32 Segments /*= 64*/, const FColor& Color /*= FColor::Green*/, float Duration /*= 0.f*/, float Thickness /*= 1.f*/);


	UPROPERTY()
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY()
	TArray<TObjectPtr<USplineMeshComponent>> SplineMeshes;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> SplineMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInterface> PreviewMaterial;

	TArray<FVector> CurrentPoints;

	FUnitAbilityPreviewInfo UnitAbilityPreviewInfo;
};
