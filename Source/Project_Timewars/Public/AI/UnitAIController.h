// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StrategyAIController.h"
#include "StrategyCommandInterface.h"

#include "UnitAIController.generated.h"

UENUM(BlueprintType)
namespace EUnitTask
{
	enum Type
	{
		Idle UMETA(DisplayName = "Idle"),
	    Moving UMETA(DisplayName = "Moving"),
	    Attacking UMETA(DisplayName = "Attacking"),
	    Building UMETA(DisplayName = "Building")
	};
}

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
	virtual void EnqueueAction(UStrategyAIAction* NewAction) override;
	// IStrategyCommandInterface END

	UFUNCTION(BlueprintCallable)
	void SetCurrentTask(EUnitTask::Type NewTask);

	UFUNCTION(BlueprintCallable)
    EUnitTask::Type GetCurrentTask() const { return CurrentTask; }

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
	UPROPERTY()
	class AUnitActor* PossessedUnit;

	EUnitTask::Type CurrentTask;
};
