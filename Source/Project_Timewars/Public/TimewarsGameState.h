// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TimewarsGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API ATimewarsGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	/** World bounds for mini map & camera movement. */
    FBox WorldBounds;
};
