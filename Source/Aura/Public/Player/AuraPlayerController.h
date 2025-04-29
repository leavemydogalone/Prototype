// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class IHighlightInterface;
class UNiagaraSystem;
class UDamageTextComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class AMagicCircle;
class AAbilityPreview;
struct FStoredAbilityInfo;
class AAuraPlayerState;

enum class ETargetingStatus : uint8
{
	TargetingEnemy,
	TargetingNonEnemy,
	NotTargeting
};

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();

	UFUNCTION(Client, Reliable)
	void ShowAbilityPreview(const FUnitAbilityPreviewInfo& UnitAbilityPreviewInfo);

	UFUNCTION(Client, Reliable)
	void HideAbilityPreview();

	//update this and PS function to use FUnitAbilityPreviewInfo
	void UpdateStoredAbilityPreviews(TArray<FStoredAbilityInfo>& StoredAbilities);

	void BindToStoredAbilitiesDelegate();

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;

	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();
	TObjectPtr<AActor> LastActor;
	TObjectPtr<AActor> ThisActor;
	FHitResult CursorHit;
	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	UAuraAbilitySystemComponent* GetASC();

	//My Additions
	UPROPERTY()
	TObjectPtr<AAuraPlayerState> AuraPlayerState;
	AAuraPlayerState* GetPS();

	
	//FVector CachedDestination = FVector::ZeroVector;
	//float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	//bool bAutoRunning = false;
	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	/*UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;*/

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	//void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	void UpdateMagicCircleLocation();

	//Ability Preview

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAbilityPreview> AbilityPreviewClass;

	UPROPERTY()
	TObjectPtr<AAbilityPreview> AbilityPreview;

	void UpdateAbilityPreviewLocation();

	bool bIsStoredAbilitiesDelegateBound = false;

	void DrawLineToMouse(AActor* Unit, int32 MaxRange);

	FUnitAbilityPreviewInfo CurrentUnitAbilityPreviewInfo;
	bool bAbilityPreviewIsActive = false;
};
