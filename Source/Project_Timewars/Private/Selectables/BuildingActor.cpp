// Fill out your copyright notice in the Description page of Project Settings.


#include "Selectables/BuildingActor.h"

ABuildingActor::ABuildingActor()
{
	ActorData.ActorType = EActorType::Building;
}

void ABuildingActor::BeginPlay()
{
	Super::BeginPlay();
}
