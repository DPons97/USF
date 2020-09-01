// Fill out your copyright notice in the Description page of Project Settings.

#include "../AI/UnitAIController.h"

#include "NavigationSystem.h"
#include "Selectables/UnitActor.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TimewarsSpectatorPawn.h"

AUnitAIController::AUnitAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData>BBObject(TEXT("BlackboardData'/Game/AI/Units_BB.Units_BB'"));
	if (!ensure(BBObject.Object != nullptr)) return;
	BBAsset = BBObject.Object;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BTObject(TEXT("BehaviorTree'/Game/AI/Units_BT.Units_BT'"));
	if (!ensure(BTObject.Object != nullptr)) return;
	BTAsset = BTObject.Object;
}

void AUnitAIController::BeginPlay()
{
	Super::BeginPlay();

}

void AUnitAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Casting possessed pawn and controller interface injection
	PossessedUnit = Cast<AUnitActor>(InPawn);
	
	BlackboardComponent->SetValueAsEnum(TEXT("CurrentTask"), EUnitTask::Idle);
	BlackboardComponent->SetValueAsVector(TEXT("CurrentDestination"), FVector(0,0,0));
	BlackboardComponent->SetValueAsVector(TEXT("NewDestination"), FVector(0,0,0));
}

void AUnitAIController::MouseRight(ATimewarsSpectatorPawn* Requestor, FVector destination, bool bOverridePreviousAction)
{	
	if (!CanPerformActions(Requestor)) return;

	// todo create new action based on type of right click
	
	FAction NewAction;
	NewAction.ActionType = Moving;
	NewAction.TargetLocation = destination;	
	
	if (!ActionsQueue.IsEmpty()) {
		if (bOverridePreviousAction)
		{
			ActionsQueue.Empty();
			ActionsQueue.Enqueue(NewAction);
			ExecuteNextAction();
		} else
		{
			ActionsQueue.Enqueue(NewAction);		
		}
	} else
	{
		ActionsQueue.Enqueue(NewAction);
		ExecuteNextAction();
	}
}

void AUnitAIController::ExecuteNextAction()
{
	// Decode next action
	FAction NextAction;
	if (ActionsQueue.Peek(NextAction))
	{
		switch (NextAction.ActionType)
		{
			case Moving:
				if (!ensure(BlackboardComponent != nullptr)) return;
				BlackboardComponent->SetValueAsVector(TEXT("NewDestination"), NextAction.TargetLocation);
				BlackboardComponent->SetValueAsEnum(TEXT("CurrentTask"), EUnitTask::Moving);
		
				SetCurrentTask(Moving);
				break;
			case Attacking:
				// todo implement
				break;
			case Building:
				// todo implement
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("Invalid action in queue"))
		} 
	}
}

void AUnitAIController::FinishCurrentAction()
{
	if (ActionsQueue.Pop())
	{
		ExecuteNextAction();		
	} else
	{
		SetCurrentTask(EUnitTask::Idle);
		BlackboardComponent->SetValueAsEnum(TEXT("CurrentTask"), EUnitTask::Idle);
	}
}

void AUnitAIController::AttackUnit()
{
}
