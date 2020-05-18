// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
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
class PROJECT_TIMEWARS_API AUnitAIController : public AAIController
{
	GENERATED_BODY()

public:
	AUnitAIController();

	UFUNCTION()
	void AttackUnit();

	UFUNCTION()
	void Move(FVector destination);
};
