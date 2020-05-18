// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StrategyUtils.h"
#include "UObject/Interface.h"
#include "StrategySelectionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStrategySelectionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_TIMEWARS_API IStrategySelectionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetActorSelected(bool isSelected);

	virtual void SetActorPreSelected(bool isPreSelected) = 0;

	bool IsSelected() const;

	FSelectableData ActorData;

protected:
	bool Selected = false;
};
