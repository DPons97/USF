// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectableActor.h"
#include "Project_Timewars/Public/StrategyCommandInterface.h"

#include "BuildingActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API ABuildingActor : public ASelectableActor, public IStrategyCommandInterface
{
	GENERATED_BODY()


public:
	ABuildingActor();

	/* Strategy interface implementations */
	void LeftMouse(FVector ClickPosition) override;
};
