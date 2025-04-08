// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraUnitBase.generated.h"

class UBehaviorTree;
class AAuraAIController;

/**
 * 
 */
UCLASS()
class AURA_API AAuraUnitBase : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraUnitBase();

	virtual void PossessedBy(AController* NewController) override;

	// Unit interface
	//virtual void HighlightActor() override;
	//virtual void UnHighlightActor() override;
	// end Unit interface


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

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
};
