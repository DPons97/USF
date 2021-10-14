// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SelectablePawn.h"
#include "ObjectQueue.h"
#include "StrategyAIAction.h"

#include "StrategyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API AStrategyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AStrategyAIController();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    class UBlackboardData* BBAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    class UBehaviorTree* BTAsset;

	void ComputePathToDestination(const FVector Destination, FNavPathSharedPtr& OutPath) const;

protected:
	void OnPossess(APawn* InPawn) override;

	bool CanPerformActions(class ASpectatorPawn* Requestor) const;

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;

	UObjectQueue<UStrategyAIAction*> ActionsQueue;

private:
	UPROPERTY()
	ASelectablePawn* SelectablePawn;
};
