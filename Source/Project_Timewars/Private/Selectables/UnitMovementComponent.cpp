// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitMovementComponent.h"

#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "UnitAIController.h"

void UUnitMovementComponent::BeginPlay()
{
	Unit = Cast<AUnitActor>(GetOwner());
	ensure(Unit != nullptr);
}

void UUnitMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	if (Unit->HasAuthority())
	{
		if (PathPoints.Num() > 0 && FVector::Dist(GetActorLocation(), PathPoints[0]) < AcceptableRadius)
		{
			PathPoints.RemoveAt(0);

			SyncLocationAndRotation(Unit->GetActorLocation(), Unit->GetActorRotation());
			if (PathPoints.Num() == 0)
			{
				StopMoveTo();
			} else
			{
				StartMoveTo(PathPoints[0]);
			}
		}
	}
	
	if (!Velocity.IsZero())
	{
		ApplyLocation(DeltaTime);
		ApplyRotation(DeltaTime);
	}
}

void UUnitMovementComponent::ApplyRotation(float DeltaSeconds)
{
	// Rotation
	const FRotator RotationDiff = (Unit->GetActorRotation() - Velocity.Rotation()).Clamp();
	if (!RotationDiff.IsNearlyZero(5.f))
	{						
		FRotator NewRotation = Unit->GetActorRotation();
		const float DeltaDegrees = Unit->ActorData.TurningSpeed * DeltaSeconds * 180.f / 3.14;
			
		NewRotation.Yaw +=  RotationDiff.Yaw < 180.f ? - DeltaDegrees : DeltaDegrees;
		NewRotation.Roll += RotationDiff.Roll < 180.f ? - DeltaDegrees : DeltaDegrees;
		NewRotation.Pitch += RotationDiff.Pitch < 180.f ? - DeltaDegrees : DeltaDegrees;

		// UE_LOG(LogTemp, Warning, TEXT("%s"), *NewRotation.ToString())

		Unit->SetActorRotation(NewRotation);
	}
}

void UUnitMovementComponent::ApplyLocation(float DeltaSeconds)
{
	// Location
	const FVector NewLocation = GetActorLocation() + Velocity*DeltaSeconds;
	Unit->SetActorLocation(NewLocation);
}

void UUnitMovementComponent::CommandNavMoveTo_Implementation(const FVector& Destination)
{
	const auto StrategyController = Unit->GetStrategyController();
	if (StrategyController == nullptr) return;
	
	TArray<FVector> resultPoints;
	if (StrategyController->SearchPath(Destination, resultPoints) && resultPoints.Num() > 0)
	{		
		ClearPathPoints();
		
		// Add and send new path
		SyncLocationAndRotation(Unit->GetActorLocation(), Unit->GetActorRotation());
		StartMoveTo(resultPoints[1]);
		for (int i = 1; i < resultPoints.Num(); i++)
		{
			PathPoints.Add(resultPoints[i]);
		}		
	}
}

bool UUnitMovementComponent::CommandNavMoveTo_Validate(const FVector& Destination)
{
	// todo implement validation
	return true;
}

void UUnitMovementComponent::StartMoveTo_Implementation(const FVector& Destination)
{
	const FVector Location = GetActorLocation();	
	Velocity = (Destination - Location).GetSafeNormal() * (Unit->ActorData.Speed * 100 / 3.6);
}

void UUnitMovementComponent::CommandNavStopMove_Implementation()
{
	ClearPathPoints();

	StopMoveTo();
}

bool UUnitMovementComponent::CommandNavStopMove_Validate()
{
	// todo implement validation
	return true;
}

void UUnitMovementComponent::StopMoveTo_Implementation()
{
	Velocity = FVector::ZeroVector;
}

void UUnitMovementComponent::ClearPathPoints()
{
	FTimerManager& TimerManager = Unit->GetWorldTimerManager();
		
	// Discard last points sending timers and empty previous path
	for (auto handle : LastPathPointsHandles)
	{
		TimerManager.ClearTimer(handle);			
	}
	LastPathPointsHandles.Empty();
	PathPoints.Empty();
}

void UUnitMovementComponent::SyncLocationAndRotation_Implementation(const FVector& ServerLocation, const FRotator ServerRotation)
{
	if (!Unit->HasAuthority())
	{
		Unit->SetActorLocation(ServerLocation);
		Unit->SetActorRotation(ServerRotation);
	}
}

FVector UUnitMovementComponent::GetCurrentDestination() const
{
	return PathPoints.Num() > 0 ? PathPoints.Last() : GetActorLocation();
}
