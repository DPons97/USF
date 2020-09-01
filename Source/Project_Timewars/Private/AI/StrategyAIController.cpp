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
	SetActorTickEnabled(false);
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
