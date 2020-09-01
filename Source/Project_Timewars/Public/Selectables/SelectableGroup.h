// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SelectableGroup.generated.h"

/**
 *	This enum represents the type associated to this group (depending on the types of units this group is composed of).
 *	Group of different types can differ in the way they handle actions.
 *	
 * 	At this moment there are only two types of group as there is only one type of AI-controlled pawns (Units)
 * 	With the addition of different types of units (e.g. Cavalry, Artillery...) this enum can be expanded with
 * 	such types.
 */
UENUM()
enum EGroupType
{
	UNDEFINED,
	Units,
	Hybrid
};

/**
*	 EFormationState::Broken means that the formation isn't formed and isn't trying to form.
*	 EFormationState::Forming signifies that our formation is trying to form up,
*	 but hasn't yet reached EFormationState::Formed.
*
*	 Once all of our units are in their desired positions, we change the formation state to EFormationState::Formed.
*/
UENUM()
enum EFormationState
{
	Broken,
    Forming,
    Formed
};

// Forward declarations
class ASelectablePawn;
class UFormationHelper;
class AAIController;
struct FNavigationPath;
class ATimewarsPlayerController;

UCLASS()
class PROJECT_TIMEWARS_API USelectableGroup : public UObject
{
	GENERATED_BODY()

public:
	USelectableGroup();

	// Returns the type of this group (see EGroupType)
	EGroupType GetType() const { return GroupType; }
	void SetType( EGroupType Type ) { GroupType = Type; }

	/**
	 * @return The maximum speed of the group (the speed of the slowest unit)
	 */
	float GetMaxSpeed() const { return MaxSpeed; }

	/**
	 *	Choose the new commander of this group.
	 *	The current heuristic is to take the nearest unit to the destination (without considering obstacles).
	 *	todo Use simpler pathfinding algorithm to compute approximation of actual walk distance  
	 *	
	 * 	@return This group commander
	 */
	ASelectablePawn* GetGroupCommander() const { return Commander; }
	void ChooseGroupCommander();

	// Id lookup for any formation attached to the group (will be some sentinel value if not set).

	void SetFormationID( uint8 FormationID ) { CurrentFormationID = FormationID; }
	uint8 GetCurrentFormationID() const { return CurrentFormationID; }

	EFormationState GetState() const { return CurrentState; }
	void SetState( EFormationState NewState ) { CurrentState = NewState; }

	// Basic unit addition and removal functions.

	void AddUnits(TArray<ASelectablePawn*> NewUnits);

	/**
	 * 	@return false if no more space is available in the current formation
	 */
	bool AddUnit( ASelectablePawn* NewUnit );

	void RemoveUnit( ASelectablePawn* ToRemove );

	int GetNumberUnits() const { return Units.Num(); }

	// Get orientation of group (i.e. the commander's orientation)
	FVector GetGroupOrientation();

	// Get location of group (i.e. the commander's location)
	FVector GetGroupLocation();

	FVector GetCurrentDestination() const { return CurrentDestination; }

	// GROUP ORDERS
	/**
	 * 	Give this group of units an order to move while using the currently selected formation
	 * 	First, each unit will be given order to regroup and take its place inside the formation.
	 * 	Then the whole group will move to the target.
	 *
	 * 	todo Regrouping asynchronously while moving
	 */
	void GiveMovementOrder(UWorld* World, FVector Destination);

	void MoveInFormation(ATimewarsPlayerController* PlayerController, int UnitIndex, FNavigationPath* Path, bool bOverridePreviousMoves,
		bool bDrawDebugPaths = false);

	FNavigationPath* ComputePathToDestination(AAIController* AIController) const;

	static FVector LocalToWorldPosition(FVector LocalPosition, FVector Centroid, FVector Orientation);

protected:
	// List of all units belonging to this group
	TArray<ASelectablePawn*> Units;

	// Position of each unit relative to the centroid of the currently selected formation
	TArray<FVector> UnitsPositions;  
	
	EGroupType GroupType;
		
	float MaxSpeed;
	
	ASelectablePawn* Commander;

	uint8 CurrentFormationID;

	EFormationState CurrentState;

	FVector CurrentDestination;
};
