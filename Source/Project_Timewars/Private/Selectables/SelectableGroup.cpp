// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectableGroup.h"


#include "DrawDebugHelpers.h"
#include "FormationHelper.h"
#include "SelectablePawn.h"
#include "StrategyAIController.h"
#include "TimewarsPlayerController.h"
#include "UnitAIController.h"
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
		const float CurrDist =  FVector::Dist(CurrentDestination, Units[i]->GetActorLocation());
		
		if (CurrDist < MinDist)
		{
			MinDist = CurrDist;
			NewCommander = Units[i];
		}
	}
	Commander = NewCommander;
}

void USelectableGroup::AddUnits(TArray<AUnitActor*> NewUnits)
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

bool USelectableGroup::AddUnit(AUnitActor* NewUnit)
{
	const int FormationRemainingCapacity = UFormationHelper::GetFormationMaxUnits(CurrentFormationID) - Units.Num();
	if (FormationRemainingCapacity == 0) return false;

	Units.Add(NewUnit);
	return true;
}

void USelectableGroup::RemoveUnit(AUnitActor* ToRemove)
{
	Units.Remove(ToRemove);

	if (Units.Num() == 0 && IsValidLowLevel())
	{
		ConditionalBeginDestroy();
	}
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

/**
 *	@param NavPath navigation path of formation
 *	@param UnitIdx index of unit that has to reach the regrouping point
 *	@param UnitSpeed base speed of this unit
 *	@param OutDistance output: total distance from this unit's position to regrouping point 
 */
float USelectableGroup::GetTimeToDestinationWithBoost(FNavPathSharedPtr NavPath, int UnitIdx, const float UnitSpeed, float& OutDistance)
{
	const FVector RegroupingPoint = LocalToWorldPosition(
		UnitsPositions[UnitIdx],
		NavPath->GetPathPoints()[1],
		NavPath->GetSegmentDirection(1));

	FNavPathSharedPtr UnitPath;
	Units[UnitIdx]->GetStrategyController()->ComputePathToDestination(RegroupingPoint, UnitPath);

	if (UnitPath.IsValid())
	{
		OutDistance = UnitPath->GetLength();
		return OutDistance / (UnitSpeed * GMax_Unit_Boost_Multiplier * GKmH_To_CmS);
	}

	UE_LOG(LogTemp, Warning, TEXT("Could not compute time to regrouping point with boost"))
	return 0.0f;
}

void USelectableGroup::GiveMovementOrder(UWorld* World, FVector Destination)
{
	if (World == nullptr) return;
	if (Units.Num() == 0 || GroupType == EGroupType::UNDEFINED) return;

	CurrentDestination = Destination;
	ChooseGroupCommander();
	
	ATimewarsPlayerController* PC = Cast<ATimewarsPlayerController>(World->GetFirstPlayerController());

	// Find Path to CurrentDestination
	FNavPathSharedPtr NavPath;
	Commander->GetStrategyController()->ComputePathToDestination(CurrentDestination, NavPath);
	if (!NavPath.IsValid()) return; 

	// Update Group orientation
	const FVector NextOrientation = NavPath.Get()->GetSegmentDirection(1);
		
	// Perform regrouping and then move together to destination preserving formation
	TArray<float> Distances;
	const float RegroupingTime = InitializeFormation(NavPath, Distances);

	for (int i = 0; i < Units.Num(); i++)
	{
		const float RegroupingSpeed = (Distances[i] / RegroupingTime) / GKmH_To_CmS;
		if (RegroupingSpeed > Units[i]->GetSpeed())
		{
			// Use slightly boosted speed
			Units[i]->SetBaseSpeed(RegroupingSpeed);
		}
		
		PC->OrderSimpleMovement(
			Units[i],
			LocalToWorldPosition(UnitsPositions[i], GetGroupLocation(), NextOrientation),
			this,
			true,
			nullptr,
			&USelectableGroup::OnUnitInPosition);

		DrawDebugPoint(
            World,
            LocalToWorldPosition(UnitsPositions[i], GetGroupLocation(), NextOrientation),
            10.f,
            FColor::Green,
            false,
            10.f);

		//	move this unit to current destination preserving formation
		MoveInFormation(PC, i, NavPath.Get(), true);
	}

	SetState(EFormationState::Forming);
}

float USelectableGroup::InitializeFormation(FNavPathSharedPtr NavPath, TArray<float>& Out_RegroupingDistance)
{
	if (Units.Num() == 0) return -1.f;

	// Generate units position based on the selected formation
	UFormationHelper::GetLocalUnitsPositions(Units, OUT UnitsPositions, 0);
	
	// Find the slowest unit and set this group's overall max speed
	MaxSpeed = Units[0]->GetSpeed();
	float MaxRegroupingTime = -1.f;
	TArray<float> Distances;
	
	for (int i = 0; i < Units.Num(); i++)
	{
		Units[i]->ResetBaseSpeed();
		
		// todo apply eventual speed modifiers
		const float UnitSpeed = Units[i]->GetSpeed();
		if (UnitSpeed < MaxSpeed)
		{
			MaxSpeed = Units[i]->GetSpeed();			
		}

		// Calculate time to get to this unit's position into formation
		float RegroupingDist;
		const float RegroupingTime = GetTimeToDestinationWithBoost(NavPath, i, UnitSpeed, RegroupingDist);
		Distances.Add(RegroupingDist);

		if (RegroupingTime > MaxRegroupingTime)
		{
			MaxRegroupingTime = RegroupingTime;
		}
	}

	UnitsInPositions = 0;
	Out_RegroupingDistance = Distances;
	return  MaxRegroupingTime;
}

void USelectableGroup::MoveInFormation(ATimewarsPlayerController* PlayerController, int UnitIndex,
    FNavigationPath* Path, bool bDrawDebugPaths)
{
	TArray<FNavPathPoint> NavPathPoints = Path->GetPathPoints();

	// Compute relative position of unit inside formation and make it orientated to zero
	TArray<FVector> UnitPath;
	for (int j = 1; j < NavPathPoints.Num(); j++)
	{
		FVector UnitPoint = LocalToWorldPosition(
            UnitsPositions[UnitIndex],
            NavPathPoints[j].Location,
            Path->GetSegmentDirection(j)
            );

		UnitPath.Add(UnitPoint);
		
		if (bDrawDebugPaths)
		{
			DrawDebugPoint(
				PlayerController->GetWorld(),
				UnitPoint,
				10.f,
				FColor::Green,
				false,
				10.f);
		}
	}

	PlayerController->OrderPathMovement(Units[UnitIndex], UnitPath, this, false);			
}

FVector USelectableGroup::LocalToWorldPosition(FVector LocalPosition, FVector Centroid, FVector Orientation)
{
	// Orientation
	LocalPosition = LocalPosition.RotateAngleAxis(Orientation.Rotation().Yaw, FVector::UpVector);

	// Translation
	LocalPosition = LocalPosition + Centroid;

	return LocalPosition;
}

void USelectableGroup::OnUnitInPosition(ASelectablePawn* Caller)
{	
	UnitsInPositions++;
	Caller->ResetBaseSpeed();

	if (UnitsInPositions == Units.Num())
	{
		UnitsInPositions = 0;
		
		for (auto u : Units)
		{
			Cast<AUnitAIController>(u->Controller)->ExecuteNextAction();
		}
		
		SetState(EFormationState::Formed);
	} else
	{
		Cast<AUnitAIController>(Caller->Controller)->SetCurrentTask(EUnitTask::Idle);
	}
}

void USelectableGroup::BeginDestroy()
{	
	for (auto u : Units)
	{
		if (u->GetGroup() == this)
			u->SetGroup(nullptr);
	}

	Super::BeginDestroy();
}
