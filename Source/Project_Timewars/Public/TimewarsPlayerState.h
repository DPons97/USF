// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StrategyUtils.h"
#include "GameFramework/PlayerState.h"
#include "TimewarsPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API ATimewarsPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ATimewarsPlayerState();
	
	/* Player color / team */
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ESurvivorColor::Type> TeamColor; 
	
	/* Current points */
	UPROPERTY(EditDefaultsOnly)
    uint32 PointsAvailable;
    
    /** current resources */
	UPROPERTY(EditDefaultsOnly)
    uint32 StoneAvailable;
	
	UPROPERTY(EditDefaultsOnly)
    uint32 WoodAvailable;

	UPROPERTY(EditDefaultsOnly)
    uint32 FoodAvailable;

    /* total points gathered */
	UPROPERTY(EditDefaultsOnly)
	uint32 PointsTotal;
    
    /** total resources gathered */
	UPROPERTY(EditDefaultsOnly)
	uint32 StoneTotal;
	
	UPROPERTY(EditDefaultsOnly)
    uint32 WoodTotal;

	UPROPERTY(EditDefaultsOnly)
    uint32 FoodTotal;

    /* todo: time machine weak pointer */

    /** player owned unit list */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<class AUnitActor*> UnitsList;
    
    /** player owned buildings list */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<class ABuildingActor*> BuildingsList;
};
