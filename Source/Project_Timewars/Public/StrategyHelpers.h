// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

/**
 * 
 */
class PROJECT_TIMEWARS_API StrategyHelpers
{
public:
	StrategyHelpers();
	~StrategyHelpers();
	
	static bool GetMousePosition(FVector2D& outMousePosition, APlayerController* PlayerController);
	static bool DeprojectPositionToWorld(FVector2D position, FVector& outWorldPosition, APlayerController* PlayerController, ECollisionChannel CollisionChannel);
};
