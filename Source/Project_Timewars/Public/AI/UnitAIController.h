// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "StrategyCommandInterface.h"
#include "UnitAIController.generated.h"

UENUM(BlueprintType)
enum EUnitTask
{
	Idle UMETA(DisplayName = "Idle"),
	Moving UMETA(DisplayName = "Moving"),
	Attacking UMETA(DisplayName = "Attacking"),
	Building UMETA(DisplayName = "Building")
};

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API AUnitAIController : public AAIController, public IStrategyCommandInterface
{
	GENERATED_BODY()

public:	
	AUnitAIController();

	UFUNCTION()
	void AttackUnit();

	UFUNCTION()
	void Move(FVector destination) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UBlackboardData* BBAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    class UBehaviorTree* BTAsset;

protected:
	void BeginPlay() override;
	
	void OnPossess(APawn* InPawn) override;

private:
	UBlackboardComponent* BlackboardComponent;
	
	class AUnitActor* PossessedUnit;	
};
