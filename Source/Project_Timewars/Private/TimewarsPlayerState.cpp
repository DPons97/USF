// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_Timewars/Public/TimewarsPlayerState.h"

ATimewarsPlayerState::ATimewarsPlayerState()
{
	TeamColor = ESurvivorColor::Blue;

	PointsAvailable = 0;
	
	StoneAvailable = 500;
	WoodAvailable = 500;
	FoodAvailable = 500;

	PointsTotal = 0;

	StoneTotal = 0;
	WoodTotal  = 0;
	FoodTotal = 0;
}

