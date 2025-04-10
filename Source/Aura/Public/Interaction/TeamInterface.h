// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TeamInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTeamInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ITeamInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// Returns the team of the actor
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetTeamID();
	
// Returns the team of the actor
	virtual void SetTeamID(int32 ID) = 0;

	// Returns true if the actor is on the same team as the other actor
	//UFUNCTION(BlueprintCallable, Category = "Team")
	//virtual bool IsOnSameTeam(AActor* OtherActor) const;


};
