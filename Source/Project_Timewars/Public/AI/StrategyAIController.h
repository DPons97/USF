// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SelectablePawn.h"
#include "ObjectQueue.h"

#include "StrategyAIController.generated.h"

class UStrategyAIAction;

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

	FNavigationPath* ComputePathToDestination(FVector Destination) const;

protected:
	void OnPossess(APawn* InPawn) override;

	bool CanPerformActions(class ASpectatorPawn* Requestor) const;
	
	UBlackboardComponent* BlackboardComponent;

	UObjectQueue<UStrategyAIAction*> ActionsQueue;	

private:
	ASelectablePawn* SelectablePawn;
};
