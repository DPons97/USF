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
	SetActorTickEnabled(false);
	SetReplicates(true);
	SetReplicatingMovement(false);

	UnitMovementComponent = Cast<UUnitMovementComponent>(GetSelectableMovement());
}

void AUnitActor::BeginPlay()
{
	Super::BeginPlay();

}

void AUnitActor::MoveTo(const FVector& Destination, float AcceptableRadius, bool DrawPath) const
{
	UnitMovementComponent->SetAcceptableRadius(AcceptableRadius);
	UnitMovementComponent->CommandNavMoveTo(Destination, DrawPath);
}

void AUnitActor::StopMoving() const
{
	UnitMovementComponent->CommandNavStopMove();
}

FVector AUnitActor::GetCurrentDestination() const
{
	return UnitMovementComponent->GetCurrentDestination();
}