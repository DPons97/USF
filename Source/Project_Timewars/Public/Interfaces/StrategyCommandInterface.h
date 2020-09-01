// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StrategyCommandInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStrategyCommandInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_TIMEWARS_API IStrategyCommandInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/** Handle Right mouse operations. Always call this method from server.
	 * @param toLocation -> Mouse position on click in World space
	 * @param Requestor -> Pointer to SpectatorPawn who performed the click action
	 */
	virtual void MouseRight(class ATimewarsSpectatorPawn* Requestor, FVector toLocation, bool bOverridePreviousAction = false) = 0;
};
