// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StrategyUtils.generated.h"

UENUM()
namespace ETeam {
	enum Type
	{
		Survivor UMETA(DisplayName = "Survivor"),
	    Zombie UMETA(DisplayNam = "Zombie"),
	    Neutral UMETA(DisplayName = "Neutral"),
	    UNDEFINED UMETA(DisplayName = "UNDEFINED")
	};
}

UENUM()
namespace EActorType
{
	enum Type
	{
		Unit,
	    Building,
	    Resource,
	    UNDEFINED
	};	
}

USTRUCT()
struct FSelectableData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category=Data)
	TEnumAsByte<EActorType::Type> ActorType;

	UPROPERTY(EditDefaultsOnly, Category=Data)
	TEnumAsByte<ETeam::Type> OwningTeam;
	
	/** minimal attack damage */
    UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 AttackMin;

	/** maximum attack damage */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 AttackMax;

	/** minimal attack damage - siege */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 AttackMinSiege;
	
	/** maximum attack damage - siege */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 AttackMaxSiege;

	/** range of pawn attack */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 AttackDistance;

	/** damage reduction */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 DamageReduction;

	/** maximum health */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    float MaxHealth;

	/** maximum health */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    float Health;

	/** health change every 5 seconds */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    float HealthRegen;

	/** movement speed */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 MaxSpeed;

	/** movement speed */
	UPROPERTY(EditDefaultsOnly, Category=Data)
    int32 Speed;

	/** defaults */
	FSelectableData()
	{
		ActorType = EActorType::UNDEFINED;
		OwningTeam = ETeam::UNDEFINED;
		AttackMin = 5.f;
		AttackMax = 10.f;
		AttackMinSiege = 2.f;
		AttackMaxSiege = 5.f;
		AttackDistance = 100;
		DamageReduction = 0.f;
		MaxHealth = 100.f;
		Health = MaxHealth;
		HealthRegen = 1.f;
		MaxSpeed = 10;
		Speed = MaxSpeed;
	}
};

struct FPlayerData
{
	/* Current points */
	uint32 PointsAvailable;
	
	/** current resources */
	uint32 StoneAvailable;
	uint32 WoodAvailable;
	uint32 FoodAvailable;

	/* total points gathered */
	uint32 PointsTotal;
	
	/** total resources gathered */
	uint32 StoneTotal;
	uint32 WoodTotal;
	uint32 FoodTotal;

	/* todo: time machine weak pointer */

	/** player owned unit list */
	TArray<TWeakObjectPtr<class AUnitActor>> UnitsList;
	
	/** player owned buildings list */
	TArray<TWeakObjectPtr<class ABuildingActor>> BuildingsList;
};
