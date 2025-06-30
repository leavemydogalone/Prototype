// Copyright Druid Mechanics


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Aura/Aura.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/HighlightInterface.h"
#include "UI/Widget/DamageTextComponent.h"
#include "Player/AuraPlayerState.h"
#include "Character/AuraUnitBase.h"
//#include "AbilitySystem/Data/UnitAbilityPreviewContext.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Actor/AbilityPreview.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	//AutoRun();
	UpdateActiveAbilityPreview();
}

//Will replace these with show Preview
void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

void AAuraPlayerController::ShowAbilityPreview_Implementation(const FUnitAbilityPreviewInfo& UnitAbilityPreviewInfo)
{
	if (!IsLocalController())
	{
		return;
	}

	if (!IsValid(AbilityPreview))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = nullptr;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AbilityPreview = GetWorld()->SpawnActor<AAbilityPreview>(
			AbilityPreviewClass,
			FTransform::Identity,
			SpawnParams
		);

		if (AbilityPreview)
		{
			// Set the abilitypreviewinfo on the AbilityPreview
			AbilityPreview->SetUnitAbilityPreviewInfo(UnitAbilityPreviewInfo);
			bAbilityPreviewIsActive = true;

		}

	}
}

void AAuraPlayerController::HideAbilityPreview_Implementation()
{
	if (IsValid(AbilityPreview))
	{
		AbilityPreview->Destroy();
	}
	bAbilityPreviewIsActive = false;
}

void AAuraPlayerController::UpdateActiveAbilityPreview()
{
	if (bAbilityPreviewIsActive && AbilityPreview)
	{
		AbilityPreview->UpdateAbilityPreview(CursorHit);
	}
}

void AAuraPlayerController::DrawLineToMouse(AActor* Unit, int32 MaxRange)
{
	FVector WorldLocation;
	FVector WorldDirection;
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		// Trace from mouse into the world
		FVector TraceEnd = WorldLocation + (WorldDirection * 10000.0f);

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility, QueryParams);
		FVector TargetPoint = bHit ? HitResult.Location : TraceEnd;

		// Clamp the target point to MaxRange from the unit
		FVector UnitLocation = Unit->GetActorLocation();
		FVector DirectionToTarget = (TargetPoint - UnitLocation).GetSafeNormal();
		float DistanceToTarget = FVector::Dist(UnitLocation, TargetPoint);

		if (DistanceToTarget > MaxRange)
		{
			TargetPoint = UnitLocation + DirectionToTarget * MaxRange;
		}

		TargetPoint.Z = UnitLocation.Z;

		// Draw clamped line
		DrawDebugLine(GetWorld(), UnitLocation, TargetPoint, FColor::Green, false, 2.0f, 0, 2.0f);
	}
}

void AAuraPlayerController::UpdateStoredAbilityPreviews_Implementation(const TArray<FUnitAbilityPreviewInfo>& StoredAbilities)
{
	if (StoredAbilities.IsEmpty()) return;
	for (const FUnitAbilityPreviewInfo& StoredAbility : StoredAbilities)
	{
	/*	if (IsValid(AbilityPreview))
		{
			AbilityPreview = GetWorld()->SpawnActor<AAbilityPreview>(AbilityPreviewClass);
			
		}*/

		// Check that the stored ability properties are valid
		if (!IsValid(StoredAbility.Unit) || StoredAbility.TargetLocation == FVector::ZeroVector)
		{
			continue;
		}
		DrawDebugLine(GetWorld(), StoredAbility.Unit->GetActorLocation(), StoredAbility.TargetLocation, FColor::Yellow, false, 10.0f, 0, 2.0f);
	}
}

//This is being called in the Character, in the OnRep_PlayerState, to ensure PS exists
void AAuraPlayerController::BindToStoredAbilitiesDelegate()
{
	GetPS()->OnStoredAbilitiesArrayChangedDelegate.AddUObject(this, &AAuraPlayerController::UpdateStoredAbilityPreviews);
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

//void AAuraPlayerController::AutoRun()
//{
//	if (!bAutoRunning) return;
//	if (APawn* ControlledPawn = GetPawn())
//	{
//		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
//		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
//		ControlledPawn->AddMovementInput(Direction);
//
//		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
//		if (DistanceToDestination <= AutoRunAcceptanceRadius)
//		{
//			bAutoRunning = false;
//		}
//	}
//}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
	}
}

void AAuraPlayerController::HighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_HighlightActor(InActor);
	}
}

void AAuraPlayerController::UnHighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>())
	{
		IHighlightInterface::Execute_UnHighlightActor(InActor);
	}
}

void AAuraPlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		UnHighlightActor(LastActor);
		UnHighlightActor(ThisActor);
		if (IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())

		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}
	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>())
	{
		ThisActor = CursorHit.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}

	if (LastActor != ThisActor)
	{
		UnHighlightActor(LastActor);
		HighlightActor(ThisActor);
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);

	/*if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (IsValid(ThisActor))
		{
			TargetingStatus = ThisActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
		}
		else
		{
			TargetingStatus = ETargetingStatus::NotTargeting;
		}
		bAutoRunning = false;
	}*/
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

	/*if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}*/



	//if (TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyDown)
	//{
	//	const APawn* ControlledPawn = GetPawn();
	//	if (FollowTime <= ShortPressThreshold && ControlledPawn)
	//	{
	//		if (IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())
	//		{
	//			IHighlightInterface::Execute_SetMoveToLocation(ThisActor, CachedDestination);
	//		}
	//		else if (GetASC() && !GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	//		{
	//			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
	//		}
	//		if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
	//		{
	//			Spline->ClearSplinePoints();
	//			for (const FVector& PointLoc : NavPath->PathPoints)
	//			{
	//				Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
	//			}
	//			if (NavPath->PathPoints.Num() > 0)
	//			{
	//				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
	//				bAutoRunning = true;
	//			}
	//		}
	//	}
	//	FollowTime = 0.f;
	//	TargetingStatus = ETargetingStatus::NotTargeting;
	//}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);

	//if (GetSelectedUnitASC() && !InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_1))
	//{
	//	GetSelectedUnitASC()->AbilityInputTagHeld(InputTag);
	//	return;
	//}
	/*if (GetPS()->GetSelectedUnit() && !InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_1))
	{

	}*/

	//if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	//{
	//	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	//	return;
	//}

	//if (TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown)
	//{
	//	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	//}
	//else
	//{
	//	FollowTime += GetWorld()->GetDeltaSeconds();
	//	if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;

	//	if (APawn* ControlledPawn = GetPawn())
	//	{
	//		const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
	//		ControlledPawn->AddMovementInput(WorldDirection);
	//	}
	//}

}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}

TObjectPtr<AAuraPlayerState> AAuraPlayerController::GetPS()
{
	if (AuraPlayerState == nullptr)
	{
		AuraPlayerState
			= GetPlayerState<AAuraPlayerState>();
	}
	return AuraPlayerState;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	//if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	//{
	//	return;
	//}
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
