// Fill out your copyright notice in the Description page of Project Settings.


#include "Selectables/ResourceActor.h"


AResourceActor::AResourceActor()
{
	ActorData.ActorType = EActorType::Resource;
	ActorData.OwningTeam = ETeam::Neutral;
}

void AResourceActor::BeginPlay()
{
	Super::BeginPlay();
}
