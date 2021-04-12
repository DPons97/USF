// Fill out your copyright notice in the Description page of Project Settings.


#include "Selectables/UnitActor.h"


#include "SelectableGroup.h"
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
	CurrentGroup = nullptr;
}

FVector AUnitActor::GetVelocity() const
{
	const UPawnMovementComponent* MovementComponent = GetMovementComponent();
	return MovementComponent ? MovementComponent->Velocity * ActorData.Speed : FVector::ZeroVector;
}

void AUnitActor::BeginPlay()
{
	Super::BeginPlay();
}