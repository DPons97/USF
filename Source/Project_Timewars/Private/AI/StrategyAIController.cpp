// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/StrategyAIController.h"

#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "SelectablePawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimewarsSpectatorPawn.h"
#include "Components/CapsuleComponent.h"

class ASpectatorPawn;

AStrategyAIController::AStrategyAIController()
{
	bAttachToPawn = true;
}

void AStrategyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!ensure(BBAsset != nullptr && BTAsset != nullptr)) return;
	UseBlackboard(BBAsset, BlackboardComponent);
	RunBehaviorTree(BTAsset);

	SelectablePawn = Cast<ASelectablePawn>(InPawn);
}

bool AStrategyAIController::SearchPath(const FVector& Destination, TArray<FVector> & OutPathPoints, bool DrawPath) const
{
	// Build query for navigation system
	FPathFindingQuery Query;
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetAcceptanceRadius(3.f);
	MoveRequest.SetGoalLocation(Destination);	
	
	const bool bValidQuery = BuildPathfindingQuery(MoveRequest, Query);

	// Get navigation system and initialize result/agent properties
	UWorld* World = GetWorld();
	if (World == nullptr) return false;
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	
	FNavAgentProperties navAgentProperties;
	navAgentProperties.AgentHeight = SelectablePawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2;
	navAgentProperties.AgentRadius = SelectablePawn->GetCapsuleComponent()->GetScaledCapsuleRadius();
	
	FPathFindingResult PathFindingResult;

	// Calculate path synchronously and populate array of points
	if (NavSys != nullptr)
	{
		PathFindingResult = NavSys->FindPathSync(navAgentProperties, Query);

		if (PathFindingResult.Result != ENavigationQueryResult::Error)
		{
			if (PathFindingResult.IsSuccessful() && PathFindingResult.Path.IsValid())
			{
				for(FNavPathPoint point : PathFindingResult.Path->GetPathPoints())
				{
					auto Location = point.Location;
					float CorrectedZCoordinate = Location.Z + SelectablePawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

					FVector CorrectedPoint(Location.X, Location.Y, CorrectedZCoordinate);
					OutPathPoints.Add(CorrectedPoint);

					if (DrawPath)
					{
						DrawDebugPoint(World, CorrectedPoint, 5.f, FColor::Green, false, 5.f);
					}
				}
			}
		}
	}

	return true;
}

bool AStrategyAIController::CanPerformActions(ASpectatorPawn* Requestor) const
{
	ASelectablePawn* PosessedPawn = Cast<ASelectablePawn>(GetPawn());
	return Requestor == PosessedPawn->GetOwnerPlayerPawn();
}
