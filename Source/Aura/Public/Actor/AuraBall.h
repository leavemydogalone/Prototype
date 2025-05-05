// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/AuraTurnPhase.h"
#include "AbilitySystemInterface.h"
#include "AuraBall.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayAbility;
class UGameplayEffect;

UCLASS()
class AURA_API AAuraBall : public AActor
	//, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	AAuraBall();

	/*virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }*/

protected:
	virtual void BeginPlay() override;

	void HandleTurnPhaseChange(EAuraTurnPhase NewTurnPhase);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball Components")
	TObjectPtr<UStaticMeshComponent> BallMesh;

	/*UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;*/

	//UPROPERTY()
	//TObjectPtr<UAttributeSet> AttributeSet;

	//void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;

	//virtual void InitAbilityActorInfo();

	//void AddCharacterAbilities();

	//virtual void InitializeDefaultAttributes() const;

	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	//TSubclassOf<UGameplayEffect> DefaultBallAttributes;

private:

   /* FVector StoredLinearVelocity;  
    FVector StoredAngularVelocity;*/

	void FreezeBall();
	void UnFreezeBall();

	//UPROPERTY(EditAnywhere, Category = "Abilities")
	//TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	//UPROPERTY(EditAnywhere, Category = "Abilities")
	//TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;



};
