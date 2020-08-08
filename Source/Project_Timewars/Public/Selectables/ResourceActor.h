// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/SelectablePawn.h"
#include "StrategyCommandInterface.h"

#include "ResourceActor.generated.h"

/**
 * 	Default ASelectableActor for natural resources. This class does not contain a movement component
 */
UCLASS()
class PROJECT_TIMEWARS_API AResourceActor : public ASelectablePawn
{
	GENERATED_BODY()


public:
	AResourceActor(const FObjectInitializer& ObjectInitializer);

protected:
    void BeginPlay() override;
};
