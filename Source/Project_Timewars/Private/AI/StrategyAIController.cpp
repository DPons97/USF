// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/StrategyAIController.h"

#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "SelectablePawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimewarsSpectatorPawn.h"

class ASpectatorPawn;

AStrategyAIController::AStrategyAIController()
{
	bAttachToPawn = true;
	SetActorTickEnabled(false);
}

void AStrategyAIController::ComputePathToDestination(const FVector Destination, FNavPathSharedPtr& OutPath) const
{
	FAIMoveRequest MoveReq(Destination);
	MoveReq.SetUsePathfinding(true);
	MoveReq.SetAllowPartialPath(true);
	MoveReq.SetProjectGoalLocation(false);
	MoveReq.SetAcceptanceRadius(4.f);
	MoveReq.SetReachTestIncludesAgentRadius(true);
	MoveReq.SetCanStrafe(true);

	FPathFindingQuery PFQuery;
	const bool bValidQuery = BuildPathfindingQuery(MoveReq, PFQuery);
	if (bValidQuery)
		FindPathForMoveRequest(MoveReq, PFQuery, OutPath);
}

void AStrategyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!ensure(BBAsset != nullptr && BTAsset != nullptr)) return;
	UseBlackboard(BBAsset, BlackboardComponent);
	RunBehaviorTree(BTAsset);

	SelectablePawn = Cast<ASelectablePawn>(InPawn);
}

bool AStrategyAIController::CanPerformActions(ASpectatorPawn* Requestor) const
{
	ASelectablePawn* PosessedPawn = Cast<ASelectablePawn>(GetPawn());
	return Requestor == PosessedPawn->GetOwnerPlayerPawn();
}
