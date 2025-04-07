// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCapsuleCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCapsuleCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:
	AAuraCapsuleCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/* Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	/* End Combat Interface */

private:
	virtual void InitAbilityActorInfo() override;
};
