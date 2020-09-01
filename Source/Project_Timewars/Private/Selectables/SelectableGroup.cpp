// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectableGroup.h"


#include "DrawDebugHelpers.h"
#include "FormationHelper.h"
#include "SelectablePawn.h"
#include "StrategyAIController.h"
#include "TimewarsPlayerController.h"
#include "Engine/World.h"

USelectableGroup::USelectableGroup()
{
	CurrentFormationID = 0;
	CurrentState = EFormationState::Broken;
	GroupType = EGroupType::UNDEFINED;
}

void USelectableGroup::ChooseGroupCommander()
{
	if (CurrentState == EFormationState::Formed) return;

	if (Units.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to choose new commander for empty group"))
		return;
	}

	ASelectablePawn* NewCommander = Units[0];
	float MinDist =  FVector::Dist(CurrentDestination, Units[0]->GetActorLocation());
	for (int i = 1; i < Units.Num(); i++)
	{
		float CurrDist =  FVector::Dist(CurrentDestination, Units[i]->GetActorLocation());
		
		if (CurrDist < MinDist)
		{
			MinDist = CurrDist;
			NewCommander = Units[i];
		}
	}
	Commander = NewCommander;
}

void USelectableGroup::AddUnits(TArray<ASelectablePawn*> NewUnits)
{	
	const int FormationRemainingCapacity = UFormationHelper::GetFormationMaxUnits(CurrentFormationID) - Units.Num();
	if (NewUnits.Num() > FormationRemainingCapacity)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Units in selection exceed maximum number of units available in formation. Filling only available spots."))

		for (int i = FormationRemainingCapacity; i < NewUnits.Num(); i++)
		{
			NewUnits.RemoveAt(i);
		}
	}
	
	Units = NewUnits;
}

bool USelectableGroup::AddUnit(ASelectablePawn* NewUnit)
{
	const int FormationRemainingCapacity = UFormationHelper::GetFormationMaxUnits(CurrentFormationID) - Units.Num();
	if (FormationRemainingCapacity == 0) return false;

	Units.Add(NewUnit);
	return true;
}

void USelectableGroup::RemoveUnit(ASelectablePawn* ToRemove)
{
	Units.Remove(ToRemove);
}

FVector USelectableGroup::GetGroupOrientation()
{
	if (!Commander) ChooseGroupCommander();

	return Commander->GetActorForwardVector();
}

FVector USelectableGroup::GetGroupLocation()
{
	if (!Commander) ChooseGroupCommander();

	return Commander->GetActorLocation();
}

void USelectableGroup::GiveMovementOrder(UWorld* World, FVector Destination)
{
	if (World == nullptr) return;
	if (Units.Num() == 0 || GroupType == EGroupType::UNDEFINED) return;

	// todo set MaxSpeed

	CurrentDestination = Destination;

	if (CurrentState != EFormationState::Formed) ChooseGroupCommander();

	ATimewarsPlayerController* PC = Cast<ATimewarsPlayerController>(World->GetFirstPlayerController());

	// CLIENT_ONLY - Find Path to CurrentDestination and get first point
	FNavigationPath* NavPath = ComputePathToDestination(Commander->GetStrategyController());
	
	// Update Group orientation
	FVector NextOrientation = NavPath->GetSegmentDirection(1);	

	if (CurrentState == EFormationState::Formed)
	{
		// Units already in formation
		// Move all units together to current destination preserving formation	
		for (int i = 0; i < Units.Num(); i++)
		{
			MoveInFormation(PC, i, NavPath, true, true);		
		}
	} else
	{
		// Need to move units into formation

		// Find all formation positions in world space
		UFormationHelper::GetLocalUnitsPositions(Units, OUT UnitsPositions, 0);

		for (int i = 0; i < Units.Num(); i++)
		{			
			PC->OrderSimpleMovement(
				Units[i],
				LocalToWorldPosition(UnitsPositions[i], GetGroupLocation(), NextOrientation),
				true);

			DrawDebugPoint(
                World,
                LocalToWorldPosition(UnitsPositions[i], GetGroupLocation(), NextOrientation),
                10.f,
                FColor::Green,
                false,
                10.f);

			//	move this unit to current destination preserving formation
			MoveInFormation(PC, i, NavPath, false, true);
		}
		
		// Create interception between commander and formation units while commander is moving to current destination

		// Move all units together to each interception point

		// Once all units in formation:
		SetState(EFormationState::Formed);
	}
}

void USelectableGroup::MoveInFormation(ATimewarsPlayerController* PlayerController, int UnitIndex,
    FNavigationPath* Path, bool bOverridePreviousMoves, bool bDrawDebugPaths)
{
	TArray<FNavPathPoint> NavPathPoints = Path->GetPathPoints();

	// Compute relative position of unit inside formation and make it orientated to zero
	TArray<FVector> UnitPath;
	for (int j = 1; j < NavPathPoints.Num(); j++)
	{
		UnitPath.Add(LocalToWorldPosition(
			UnitsPositions[UnitIndex],
			NavPathPoints[j].Location,
			Path->GetSegmentDirection(j)
			));

		if (bDrawDebugPaths)
		{
			DrawDebugPoint(
				PlayerController->GetWorld(),
				UnitPath[j-1],
				10.f,
				FColor::Green,
				false,
				10.f);
		}

		PlayerController->OrderPathMovement(Units[UnitIndex], UnitPath, bOverridePreviousMoves);
	}
}

FNavigationPath* USelectableGroup::ComputePathToDestination(AAIController* AIController) const
{
	FAIMoveRequest MoveReq(CurrentDestination);
	MoveReq.SetUsePathfinding(true);
	MoveReq.SetAllowPartialPath(true);
	MoveReq.SetProjectGoalLocation(false);
	MoveReq.SetAcceptanceRadius(4.f);
	MoveReq.SetReachTestIncludesAgentRadius(true);
	MoveReq.SetCanStrafe(true);

	FPathFindingQuery PFQuery;
	const bool bValidQuery = AIController->BuildPathfindingQuery(MoveReq, PFQuery);
	if (bValidQuery)
	{
		FNavPathSharedPtr Path;
		AIController->FindPathForMoveRequest(MoveReq, PFQuery, Path);

		return Path.Get();
	}

	return nullptr;
}

FVector USelectableGroup::LocalToWorldPosition(FVector LocalPosition, FVector Centroid, FVector Orientation)
{
	// Orientation
	LocalPosition = LocalPosition.RotateAngleAxis(Orientation.Rotation().Yaw, FVector::UpVector);

	// Translation
	LocalPosition = LocalPosition + Centroid;

	return LocalPosition;
}
