// Fill out your copyright notice in the Description page of Project Settings.


#include "Selectables/UnitActor.h"
#include "UnitAIController.h"
#include "Components/SkeletalMeshComponent.h"

AUnitActor::AUnitActor()
{
	AIControllerClass = AUnitAIController::StaticClass();
	ActorData.ActorType = EActorType::Unit;
	CurrentTask = Idle;
}

void AUnitActor::BeginPlay()
{
	Super::BeginPlay();
}