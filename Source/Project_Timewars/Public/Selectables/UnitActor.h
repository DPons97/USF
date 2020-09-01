// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/SelectablePawn.h"
#include "UnitActor.generated.h"

class USelectableGroup;
class UAnimationAsset;
class UAnimSequence;
class UUnitMovementComponent;
class AStrategyAIController;

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API AUnitActor : public ASelectablePawn
{
	GENERATED_BODY()

public:
	AUnitActor(const FObjectInitializer& ObjectInitializer);

	USelectableGroup* GetGroup() const { return CurrentGroup; }

	void SetGroup(USelectableGroup* NewGroup) { this->CurrentGroup = NewGroup; }

protected:
    void BeginPlay() override;

private:
	UUnitMovementComponent* UnitMovementComponent;

	// GroupID == 0 is reserved for units without a group
	USelectableGroup* CurrentGroup;	
};
