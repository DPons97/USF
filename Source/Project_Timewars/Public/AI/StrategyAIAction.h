// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SelectablePawn.h"
#include "TimewarsSpectatorPawn.h"
#include "UObject/NoExportTypes.h"
#include "StrategyAIAction.generated.h"


DECLARE_DELEGATE_OneParam( FActionStarted, ASelectablePawn* )
DECLARE_DELEGATE_OneParam( FActionEnded, ASelectablePawn* )

class ATimewarsSpectatorPawn;

/**
 * 
 */
UCLASS(Abstract)
class PROJECT_TIMEWARS_API UStrategyAIAction : public UObject
{
	GENERATED_BODY()
	
public:	
	bool bOverridePreviousAction;

	UPROPERTY()
	ATimewarsSpectatorPawn* Requestor;

	FActionStarted OnActionStartedDelegate;

	FActionEnded OnActionEndedDelegate;

	UStrategyAIAction()
		: bOverridePreviousAction(false),
		  Requestor(nullptr)
	{
	}

	/**
	 * 	Bind a function to be fired once this action is actually started by the pawn
	 */
	template <typename UserClass, typename... VarTypes>
	void BindOnActionStarted(UserClass* Object, typename FActionStarted::TUObjectMethodDelegate< UserClass >::FMethodPtr Func)
	{
		OnActionStartedDelegate.BindUObject(Object, Func);
	}

	/**
	* 	Bind a function to be fired once this action is actually dequeued by the pawn (after execution)
	*/
	template <typename UserClass, typename... VarTypes>
	void BindOnActionEnded(UserClass* Object, typename FActionEnded::TUObjectMethodDelegate< UserClass >::FMethodPtr Func)
	{
		OnActionEndedDelegate.BindUObject(Object, Func);
	}
};
