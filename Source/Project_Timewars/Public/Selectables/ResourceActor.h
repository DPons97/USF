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
class PROJECT_TIMEWARS_API AResourceActor : public ASelectablePawn, public IStrategyCommandInterface
{
	GENERATED_BODY()


public:
	AResourceActor();

	/* Strategy interface implementations */
	void Move(FVector ClickPosition) override;

protected:
    void BeginPlay() override;
};
