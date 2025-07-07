// Copyright Scitcat Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RoundManagerInterface.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoundChange, int32 NewRound);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoundChangeAssignable, int32, NewRound);


// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable)
class URoundManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IRoundManagerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FOnRoundChange& GetOnRoundChangeDelegate() = 0;

	UFUNCTION()
	virtual void StartRound() = 0;

	UFUNCTION()
	virtual void StartRoundTime() = 0;

	//UFUNCTION(BlueprintCallable, Category = "Round System")
	//virtual int32 GetCurrentRoundTime() const = 0;

	UFUNCTION(BlueprintCallable, Category = "Round System")
	virtual int32 GetCurrentRound() const = 0;

	UFUNCTION()
	virtual void AdvanceCurrentRound() = 0;

	UFUNCTION(BlueprintCallable, Category = "Round System")
	virtual int32 GetNumberOfRounds() = 0;
};
