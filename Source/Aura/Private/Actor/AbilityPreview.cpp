// Copyright Scitcat Games


#include "Actor/AbilityPreview.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

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

void AAbilityPreview::UpdateAbilityPreview(const FVector& TargetLocation)
{
    DrawDebugCircleAroundActor(UnitAbilityPreviewInfo.Unit, UnitAbilityPreviewInfo.AbilityRange, 20, FColor::Blue, 0.1f, 1.f);

    //draw debug spheres at target location and startlocation
	DrawDebugSphere(GetWorld(), TargetLocation, 20.f, 12, FColor::Red, false, 0.1f);
	DrawDebugSphere(GetWorld(), UnitAbilityPreviewInfo.Unit->GetActorLocation(), 20.f, 12, FColor::Green, false, 0.1f);

    switch (UnitAbilityPreviewInfo.AbilityPreviewType)
    {
        case
        EUnitAbilityPreviewType::Movement:
        {
            FVector OutLocation;
            const bool TargetPointValid = UAuraAbilitySystemLibrary::GetReachablePointWithinMaxRange(UnitAbilityPreviewInfo.Unit, UnitAbilityPreviewInfo.Unit->GetActorLocation(), TargetLocation, UnitAbilityPreviewInfo.AbilityRange, OutLocation);

            if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(UnitAbilityPreviewInfo.Unit, UnitAbilityPreviewInfo.Unit->GetActorLocation(), OutLocation))
            {
                for (FVector& Point : NavPath->PathPoints)
                {
                    Point.Z = UnitAbilityPreviewInfo.Unit->GetActorLocation().Z; // Ensure the path points are at the same height as the unit
                }
                UpdateSpline(NavPath->PathPoints);
            }
            break;
        }
        case
        EUnitAbilityPreviewType::RangedAttack:
        {
            FVector OutLocation;
            const bool TargetPointValid = UAuraAbilitySystemLibrary::GetReachablePointWithinMaxRange(UnitAbilityPreviewInfo.Unit, UnitAbilityPreviewInfo.Unit->GetActorLocation(), TargetLocation, UnitAbilityPreviewInfo.AbilityRange, OutLocation);

            if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(UnitAbilityPreviewInfo.Unit, UnitAbilityPreviewInfo.Unit->GetActorLocation(), OutLocation))
            {

               UpdateSpline(NavPath->PathPoints);
            }
            break;
        }
        default:
            break;
    }
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
            Mesh->RegisterComponent();
            Mesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
       
            SplineMeshes.Add(Mesh);
        }

        Mesh->SetMaterial(0, PreviewMaterial);


        FVector StartPos, StartTangent, EndPos, EndTangent;
        SplineComponent->GetLocationAndTangentAtSplinePoint(i, StartPos, StartTangent, ESplineCoordinateSpace::Local);
        SplineComponent->GetLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent, ESplineCoordinateSpace::Local);

        Mesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
        Mesh->SetStartScale(FVector2D(1.f, 1.f));
        Mesh->SetEndScale(FVector2D(1.f, 1.f));
        Mesh->SetVisibility(true);
    }
}

void AAbilityPreview::DrawDebugCircleAroundActor(AActor* TargetActor, float MaxRange, int32 Segments, const FColor& Color, float Duration, float Thickness)
{
    if (!TargetActor || !GetWorld()) return;

    //FVector Center = TargetActor->GetActorLocation();
    FVector Center = FVector(TargetActor->GetActorLocation().X, TargetActor->GetActorLocation().Y, 0.f);
    FVector UpVector = FVector::UpVector;
    FVector ForwardVector = TargetActor->GetActorForwardVector();
    FVector RightVector = FVector::CrossProduct(UpVector, ForwardVector);

    DrawDebugCircle(
        GetWorld(),
        Center,
        MaxRange,
        Segments,
        Color,
        false,        // persistent lines
        Duration,
        0,            // depth priority
        Thickness,
        RightVector,  // X-axis vector
        ForwardVector // Y-axis vector
    );
}




