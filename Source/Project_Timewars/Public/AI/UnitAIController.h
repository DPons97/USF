// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StrategyAIController.h"
#include "StrategyCommandInterface.h"
#include "Containers/Queue.h"

#include "UnitAIController.generated.h"

UENUM(BlueprintType)
enum EUnitTask
{
	Idle UMETA(DisplayName = "Idle"),
	Moving UMETA(DisplayName = "Moving"),
	Attacking UMETA(DisplayName = "Attacking"),
	Building UMETA(DisplayName = "Building")
};

USTRUCT()
struct FAction
{
	GENERATED_USTRUCT_BODY()
	
	EUnitTask ActionType;

	FVector TargetLocation;

	ASelectablePawn* TargetPawn = nullptr;
};

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API AUnitAIController : public AStrategyAIController, public IStrategyCommandInterface
{
	GENERATED_BODY()

public:	
	AUnitAIController();

	UFUNCTION()
	void AttackUnit();

	// IStrategyCommandInterface BEGIN
	virtual void MouseRight(ATimewarsSpectatorPawn* Requestor, FVector destination, bool bOverridePreviousAction) override;
	// IStrategyCommandInterface END

	UFUNCTION(BlueprintCallable)
	void SetCurrentTask(enum EUnitTask NewTask) { CurrentTask = NewTask; }

	UFUNCTION(BlueprintCallable)
    EUnitTask GetCurrentTask() const { return CurrentTask; }

	UFUNCTION(BlueprintCallable)
	void ExecuteNextAction();

	/**
	 * 	Finish to execute the current action, dequeuing the action from the queue and calling ExecuteNextAction()
	 */
	UFUNCTION(BlueprintCallable)
	void FinishCurrentAction();
	
protected:
	void BeginPlay() override;
	
	void OnPossess(APawn* InPawn) override;

private:
	class AUnitActor* PossessedUnit;

	EUnitTask CurrentTask;

	TQueue<FAction> ActionsQueue;
};
