// Fill out your copyright notice in the Description page of Project Settings.


#include "Selectables/UnitActor.h"

#include "UnitAIController.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
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

void AUnitActor::BeginPlay()
{
	Super::BeginPlay();

}

void AUnitActor::Tick(float DeltaSeconds)
{
	if (PathPoints.Num() > 0)
	{
		// todo tweak acceptance radius
		if (FVector::Dist(GetActorLocation(), PathPoints[0]) > 1.f)
		{
			FVector NewLocation = GetActorLocation() + Velocity*DeltaSeconds;
			SetActorLocation(NewLocation);
		} else
		{
			PathPoints.Pop();
			if (PathPoints.Num() > 0)
			{
				StartMoveTo(PathPoints[0]);				
			}
		}
	}
}

void AUnitActor::CommandNavMoveTo_Implementation(const FVector& Destination)
{
	const auto StrategyController = GetStrategyController();
	if (StrategyController == nullptr) return;

	TArray<FVector> resultPoints;
	if (StrategyController->SearchPath(Destination, resultPoints) && resultPoints.Num() > 0)
	{
		FTimerManager& TimerManager = GetWorldTimerManager();
		
		// Discard last points sending timers and empty previous path
		for (auto handle : LastPathPointsHandles)
		{
			TimerManager.ClearTimer(handle);			
		}
		PathPoints.Empty();		

		// Add and send new path
		StartMoveTo(resultPoints[1]);
		for (int i = 1; i < resultPoints.Num(); i++)
		{
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &AUnitActor::AddNewPathPoint, resultPoints[i]), 0.1f, false);
			LastPathPointsHandles.Add(TimerHandle);
		}
	}
}

bool AUnitActor::CommandNavMoveTo_Validate(const FVector& Destination)
{
	return true;
}

void AUnitActor::StartMoveTo_Implementation(const FVector& FirstPoint)
{
	const FVector Location = GetActorLocation();
	Velocity = (FirstPoint - Location).GetSafeNormal() * ActorData.Speed;
}

void AUnitActor::AddNewPathPoint_Implementation(FVector NewPoint)
{
	PathPoints.Add(NewPoint);
}
