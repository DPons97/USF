// Fill out your copyright notice in the Description page of Project Settings.


#include "Selectables/UnitActor.h"

#include "UnitAIController.h"
#include "Selectables/UnitMovementComponent.h"

AUnitActor::AUnitActor(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass(Super::SelectableMovementComponentName, UUnitMovementComponent::StaticClass()))
{
	// todo add AZombieAIController
	AIControllerClass = AUnitAIController::StaticClass();
	ActorData.ActorType = EActorType::Unit;
	CurrentTask = Idle;

	UnitMovementComponent = Cast<UUnitMovementComponent>(GetSelectableMovement());
}

void AUnitActor::MoveTo(const FVector& Destination) const
{
	UnitMovementComponent->CommandNavMoveTo(Destination);
}

void AUnitActor::BeginPlay()
{
	Super::BeginPlay();

}

void AUnitActor::Tick(float DeltaSeconds)
{	
	
}