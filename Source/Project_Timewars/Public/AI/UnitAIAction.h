// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StrategyAIAction.h"
#include "UnitAIAction.generated.h"

UENUM(BlueprintType)
namespace EUnitActionType
{
	enum Type
	{
		Idle UMETA(DisplayName = "Idle"),
	    MoveTo UMETA(DisplayName = "Move to"),
	    Attack UMETA(DisplayName = "Attack"),
	    Build UMETA(DisplayName = "Build")
	};
}

class ASelectablePawn;

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API UUnitAIAction : public UStrategyAIAction
{
	GENERATED_BODY()

public:


	UUnitAIAction()
		: ActionType(EUnitActionType::Idle),
		  TargetLocation(FVector(0,0,0)),
		  TargetPawn(nullptr)
	{
	}

	/**
	 * 	Default helper function to create a new action for a unit
	 * 	@param nRequestor Who is requesting this action (pawn)
	 * 	@param nActionType Type of the action to perform
	 * 	@param nTargetLocation The action will target this location if it is a MoveTo or Build action.
	 * 								For Attacks, it will be ignored.
	 * 	@param nTargetPawn Pawn to perform this action on. If action type is Attack, this will be the attacked unit.
	 * 							For MoveTo actions, this unit will follow the Target pawn.
	 *	@param bOverridePreviousAction When this is set to true the action will empty the queue of current actions
	 *										before executing this one.
	 * 		
	 *	@return The newly created action
	 *
	 *	See function BindOnActionStarted() and BindOnActionEnded() to bind specified functions and fire them once the
	 *	action is started or ended.
	 */
	static UUnitAIAction* CreateAction (ATimewarsSpectatorPawn* nRequestor, EUnitActionType::Type nActionType,
		const FVector& nTargetLocation, ASelectablePawn* nTargetPawn = nullptr, bool bOverridePreviousAction = false)
	{
		UUnitAIAction* NewAction = NewObject<UUnitAIAction>();
		NewAction->ActionType = nActionType;
		NewAction->TargetLocation = nTargetLocation;
		NewAction->TargetPawn = nTargetPawn;
		NewAction->Requestor = nRequestor;
		NewAction->bOverridePreviousAction = bOverridePreviousAction;
		
		// UE_LOG(LogTemp, Warning, TEXT("Constructing UnitAIAction %d, TargetLocation = %s"), nActionType, *nTargetLocation.ToString())

		return NewAction;
	}

	EUnitActionType::Type ActionType;

	FVector TargetLocation;

	ASelectablePawn* TargetPawn;

	FString ToString()
	{
		return FString::Printf(TEXT("Action requested by %ls: %d"), *Requestor->GetName(), ActionType);
    }

	virtual void BeginDestroy() override
	{
		// UE_LOG(LogTemp, Warning, TEXT("Destroying UnitAIAction %d, TargetLocation = %s"), ActionType, *TargetLocation.ToString())

		Super::BeginDestroy();
	}
};
