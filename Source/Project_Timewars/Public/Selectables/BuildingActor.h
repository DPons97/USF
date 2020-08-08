// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/SelectablePawn.h"
#include "StrategyCommandInterface.h"

#include "BuildingActor.generated.h"

/**
 * 	Default ASelectableActor for buildings. This class does not contain a movement component
 */
UCLASS()
class PROJECT_TIMEWARS_API ABuildingActor : public ASelectablePawn
{
	GENERATED_BODY()


public:
	ABuildingActor(const FObjectInitializer& ObjectInitializer);

protected:
	void BeginPlay() override;
};
