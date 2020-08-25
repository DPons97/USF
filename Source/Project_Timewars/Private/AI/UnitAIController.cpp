// Fill out your copyright notice in the Description page of Project Settings.

#include "../AI/UnitAIController.h"
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

void AUnitAIController::AttackUnit()
{
}

void AUnitAIController::MouseRight(ATimewarsSpectatorPawn* Requestor, FVector destination)
{
	if (!ensure(BlackboardComponent != nullptr)) return;
	if (!CanPerformActions(Requestor)) return;
	
	BlackboardComponent->SetValueAsVector(TEXT("NewDestination"), destination);
	BlackboardComponent->SetValueAsEnum(TEXT("CurrentTask"), EUnitTask::Moving);
	
	SetCurrentTask(Moving); 
}

