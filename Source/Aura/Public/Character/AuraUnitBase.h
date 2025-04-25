// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/UnitInterface.h"
#include "Interaction/TeamInterface.h"
#include "Interaction/HighlightInterface.h"
#include "AuraUnitBase.generated.h"

class UBehaviorTree;
class AAuraAIController;
struct FGameplayTag;

/**
 * 
 */
UCLASS()
class AURA_API AAuraUnitBase : public AAuraCharacterBase, public IUnitInterface, public IHighlightInterface
{
	GENERATED_BODY()
	
public:
	AAuraUnitBase();

	//virtual void PossessedBy(AController* NewController) override;

	/** Highlight Interface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	/** end Highlight Interface */

	virtual void ShowAbilityPreview_Implementation(FGameplayTag AbilityTag, FVector TargetLocation) override;

	UFUNCTION(Client, Reliable)
	virtual void Client_ShowAbilityPreview(FGameplayTag AbilityTag, FVector TargetLocation);

protected:

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	//ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	//UPROPERTY()
	//TObjectPtr<AAuraAIController> AuraAIController;

};
