// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/SelectablePawn.h"
#include "StrategyCommandInterface.h"

#include "ResourceActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API AResourceActor : public ASelectablePawn
{
	GENERATED_BODY()


public:
	AResourceActor();

protected:
    void BeginPlay() override;
};
