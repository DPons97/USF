// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/StrategyAIController.h"


#include "NavigationSystem.h"
#include "SelectablePawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimewarsSpectatorPawn.h"

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
}

bool AStrategyAIController::SearchPath(const FVector& Destination, TArray<FVector> & OutPathPoints) const
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
	UNavigationSystemV1* NavSys = Cast<UNavigationSystemV1>(World->GetNavigationSystem());

	FNavAgentProperties navAgentProperties;
	navAgentProperties.AgentHeight = 100;
	navAgentProperties.AgentRadius = 50;
	navAgentProperties.bCanWalk = true;
	navAgentProperties.bCanFly = false;
	
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

					OutPathPoints.Add(point.Location);

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


