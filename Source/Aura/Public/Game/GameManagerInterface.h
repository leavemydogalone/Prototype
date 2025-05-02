// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameManagerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IGameManagerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual int8 GetRemainingGameTime() = 0;

	UFUNCTION()
	virtual int8 GetRemainingQuarterTime() = 0;

	UFUNCTION()
	virtual int8 GetRemainingRoundTime() = 0;

	UFUNCTION()
	virtual int8 GetQuarter() = 0;



private:

};
