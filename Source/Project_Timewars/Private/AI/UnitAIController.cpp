// Fill out your copyright notice in the Description page of Project Settings.

#include "../AI/UnitAIController.h"

#include "NavigationSystem.h"
#include "Selectables/UnitActor.h"
#include "AI/UnitAIAction.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "ObjectQueue.h"

AUnitAIController::AUnitAIController() :
	PossessedUnit(nullptr),
	CurrentTask(EUnitTask::Idle)
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

void AUnitAIController::EnqueueAction(UStrategyAIAction* NewAction)
{	
	if (!CanPerformActions(NewAction->Requestor)) return;

	// todo create new action based on type of right click
	
	UUnitAIAction* UnitAction = Cast<UUnitAIAction>(NewAction);

	if (!UnitAction)
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to queue a non-unit action to this unit"))
		return;
	}
	
	if (!ActionsQueue.IsEmpty()) {
		if (UnitAction->bOverridePreviousAction)
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

void AUnitAIController::SetCurrentTask(EUnitTask::Type NewTask)
{
	CurrentTask = NewTask;
	BlackboardComponent->SetValueAsEnum(TEXT("CurrentTask"), NewTask);
}

void AUnitAIController::ExecuteNextAction()
{
	// Decode next action
	UStrategyAIAction* NextAction;

	if (ActionsQueue.Peek(NextAction))
	{
		if (!NextAction) return;
		UUnitAIAction* UnitNextAction = Cast<UUnitAIAction>(NextAction);
		if (!UnitNextAction) return;

		bool bStartDelegateFired = NextAction->OnActionStartedDelegate.ExecuteIfBound(PossessedUnit);
		
		switch (UnitNextAction->ActionType)
		{
			case EUnitActionType::MoveTo:				
				if (!ensure(BlackboardComponent != nullptr)) return;
				BlackboardComponent->SetValueAsVector(TEXT("CurrentDestination"), UnitNextAction->TargetLocation);
				SetCurrentTask(EUnitTask::Moving);
			
				break;
			case EUnitActionType::Attack:
				// todo implement
				break;
			case EUnitActionType::Build:
				// todo implement
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("Invalid action in queue"))
		} 
	}
}

void AUnitAIController::FinishCurrentAction()
{	
	UStrategyAIAction* CurrentAction;
	bool bEndDelegateFired;
	
	if (ActionsQueue.Dequeue(CurrentAction))
		bEndDelegateFired = CurrentAction->OnActionEndedDelegate.ExecuteIfBound(PossessedUnit);

	// If unit is in idle state, it has to stay that way
	if (CurrentTask == EUnitTask::Idle) return;
	
	if (ActionsQueue.Peek())
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
