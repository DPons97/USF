// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UnitActor.h"
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
namespace EGroupType
{
	enum Type
	{
		UNDEFINED,
	    Units,
	    Hybrid
	};
}

/**
*	 EFormationState::Broken means that the formation isn't formed and isn't trying to form.
*	 EFormationState::Forming signifies that our formation is trying to form up,
*	 but hasn't yet reached EFormationState::Formed.
*
*	 Once all of our units are in their desired positions, we change the formation state to EFormationState::Formed.
*/
UENUM()
namespace EFormationState
{
	enum Type
	{
		Broken,
		Forming,
		Formed
	};
}

const float GMax_Unit_Boost_Multiplier = 1.5;
const float GMax_Unit_Slow_Multiplier = 0.33;
const float GKmH_To_CmS = 100 / 3.6;

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
	EGroupType::Type GetType() const { return GroupType; }
	void SetType( EGroupType::Type Type ) { GroupType = Type; }

	/**
	 * @return The maximum speed of the group (the speed of the slowest unit)
	 */
	float GetMaxSpeed() const { return MaxSpeed; }

	/***
	 * 	Computes this group's max speed, all the units positions inside the formation and returns the maximum time
	 * 	this group will take to regroup into the selected formation.
	 * 	Such regrouping will finish on the first intermediate path point to the destination. (if no intermediate points,
	 * 	the regrouping is performed on the arrival at the destination)
	 * 	todo can generate an additional intermediate point in the first path segment
	 *
	 * 	@param NavPath the navigation path this group has to perform
	 * 	@param Out_RegroupingDistance times, for each unit, taken to arrive at regrouping point
	 */
	float InitializeFormation(FNavigationPath* NavPath, TArray<float>& Out_RegroupingDistance);

	float GetTimeToDestinationWithBoost(FNavigationPath* NavPath, int UnitIdx, float UnitSpeed, float& Distance);

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

	EFormationState::Type GetState() const { return CurrentState; }
	void SetState( EFormationState::Type NewState ) { CurrentState = NewState; }

	// Basic unit addition and removal functions.

	void AddUnits(TArray<AUnitActor*> NewUnits);

	/**
	 * 	@return false if no more space is available in the current formation
	 */
	bool AddUnit( AUnitActor* NewUnit );

	void RemoveUnit( AUnitActor* ToRemove );

	int GetNumberUnits() const { return Units.Num(); }

	// Get orientation of group (i.e. the commander's orientation)
	FVector GetGroupOrientation();

	// Get location of group (i.e. the commander's location)
	FVector GetGroupLocation();

	FVector GetCurrentDestination() const { return CurrentDestination; }

	// GROUP ORDERS
	/**
	 * 	Give this group of units an order to move while using the currently selected formation
	 * 	First, each unit will be given order to regroup and take its place inside the formation, meanwhile the
	 * 	already-in-place units will start to move to the target
	 */
	void GiveMovementOrder(UWorld* World, FVector Destination);

	/**
	 * 	Enqueues (without overriding the previous actions) the given path to the selected unit preserving its position
	 * 	in the formation
	 */
	void MoveInFormation(ATimewarsPlayerController* PlayerController, int UnitIndex, FNavigationPath* Path,
							bool bDrawDebugPaths = false);

	static FVector LocalToWorldPosition(FVector LocalPosition, FVector Centroid, FVector Orientation);

	/**
	 * 	Function called when unit is in its positions inside the formation
	 * 	(still not in the right relative position to the commander)
	 */
	void OnUnitInPosition(ASelectablePawn* Caller);

	// todo on destroy group reset all units' speeds to MaxSpeed

	virtual void BeginDestroy() override;
protected:
	// List of all units belonging to this group
	TArray<AUnitActor*> Units;

	// Position of each unit relative to the centroid of the currently selected formation
	TArray<FVector> UnitsPositions;  
	
	EGroupType::Type GroupType;
		
	float MaxSpeed;
	
	ASelectablePawn* Commander;

	uint8 CurrentFormationID;

	EFormationState::Type CurrentState;

	FVector CurrentDestination;

	// Amount of units that took their position in the formation
	int UnitsInPositions = 0;
};
