// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/StrategyAIController.h"

#include "SelectablePawn.h"
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

bool AStrategyAIController::CanPerformActions(ASpectatorPawn* Requestor) const
{
	ASelectablePawn* PosessedPawn = Cast<ASelectablePawn>(GetPawn());
	return Requestor == PosessedPawn->GetOwnerPlayerPawn();
}


