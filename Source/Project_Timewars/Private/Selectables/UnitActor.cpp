// Fill out your copyright notice in the Description page of Project Settings.


#include "Selectables/UnitActor.h"
#include "UnitAIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Selectables/UnitMovementComponent.h"

AUnitActor::AUnitActor(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass(Super::SelectableMovementComponentName, UUnitMovementComponent::StaticClass()))
{
	AIControllerClass = AUnitAIController::StaticClass();
	ActorData.ActorType = EActorType::Unit;
	CurrentTask = Idle;
}

void AUnitActor::BeginPlay()
{
	Super::BeginPlay();
}