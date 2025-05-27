// Copyright Scitcat Games


#include "Actor/AbilityPreview.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
AAbilityPreview::AAbilityPreview()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
    SetRootComponent(SplineComponent);

  

   /* AbiltyTargetDecal = CreateDefaultSubobject<UDecalComponent>("AbiltyTargetDecal");
    AbiltyTargetDecal->SetupAttachment(GetRootComponent());

	AbilityRangeDecal = CreateDefaultSubobject<UDecalComponent>("AbilityRangeDecal");
	AbilityRangeDecal->SetupAttachment(GetRootComponent());*/
}

// Called when the game starts or when spawned
void AAbilityPreview::BeginPlay()
{
    Super::BeginPlay();
}

void AAbilityPreview::UpdateSpline(const TArray<FVector>& Points)
{
    if (Points.Num() < 2) return;

    CurrentPoints = Points;

    // Update spline points
    SplineComponent->ClearSplinePoints();
    for (int32 i = 0; i < Points.Num(); ++i)
    {
        SplineComponent->AddSplinePoint(Points[i], ESplineCoordinateSpace::World);
    }
    SplineComponent->UpdateSpline();

    RebuildSplineMeshSegments();
}

void AAbilityPreview::RebuildSplineMeshSegments()
{
    if (!SplineMesh || !PreviewMaterial) return;

    int32 NeededSegments = CurrentPoints.Num() - 1;

    // Hide unused meshes
    for (int32 i = NeededSegments; i < SplineMeshes.Num(); ++i)
    {
        if (SplineMeshes[i])
        {
            SplineMeshes[i]->SetVisibility(false);
        }
    }

    // Create or reuse segments
    for (int32 i = 0; i < NeededSegments; ++i)
    {
        USplineMeshComponent* Mesh = nullptr;

        if (i < SplineMeshes.Num())
        {
            Mesh = SplineMeshes[i];
        }
        else
        {
            Mesh = NewObject<USplineMeshComponent>(this);
            Mesh->SetMobility(EComponentMobility::Movable);
            Mesh->SetStaticMesh(SplineMesh);
            Mesh->SetMaterial(0, PreviewMaterial);
            Mesh->RegisterComponent();
            Mesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
       
            SplineMeshes.Add(Mesh);
        }

        FVector StartPos, StartTangent, EndPos, EndTangent;
        SplineComponent->GetLocationAndTangentAtSplinePoint(i, StartPos, StartTangent, ESplineCoordinateSpace::Local);
        SplineComponent->GetLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent, ESplineCoordinateSpace::Local);

        Mesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
        Mesh->SetStartScale(FVector2D(1.f, 1.f));
        Mesh->SetEndScale(FVector2D(1.f, 1.f));
        Mesh->SetVisibility(true);
    }
}




