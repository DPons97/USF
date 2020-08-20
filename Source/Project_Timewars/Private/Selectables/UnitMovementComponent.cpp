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
	if (PathPoints.Num() > 0)
	{
		// todo tweak acceptance radius
		if (FVector::Dist(GetActorLocation(), PathPoints[0]) > 10.f)
		{
			ApplyLocation(DeltaTime);
			ApplyRotation(DeltaTime);
		} else
		{
			PathPoints.RemoveAt(0);
			if (PathPoints.Num() > 0)
			{
				StartMoveTo(PathPoints[0]);				
			} else
			{
				Unit->SetCurrentTask(EUnitTask::Idle);
			}
		}
	} else if (Unit->GetCurrentTask() == EUnitTask::Moving)
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
		FTimerManager& TimerManager = Unit->GetWorldTimerManager();
		
		// Discard last points sending timers and empty previous path
		for (auto handle : LastPathPointsHandles)
		{
			TimerManager.ClearTimer(handle);			
		}
		LastPathPointsHandles.Empty();
		PathPoints.Empty();		

		// Add and send new path
		StartMoveTo(resultPoints[1]);
		for (int i = 1; i < resultPoints.Num(); i++)
		{
			FTimerHandle TimerHandle;
			Unit->GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UUnitMovementComponent::AddNewPathPoint, resultPoints[i]), 0.1f, false);
			LastPathPointsHandles.Add(TimerHandle);
		}		
	}
}

bool UUnitMovementComponent::CommandNavMoveTo_Validate(const FVector& Destination)
{
	return true;
}

void UUnitMovementComponent::StartMoveTo_Implementation(const FVector& FirstPoint)
{
	const FVector Location = GetActorLocation();	
	Velocity = (FirstPoint - Location).GetSafeNormal() * (Unit->ActorData.Speed * 100 / 3.6);

	// Draw velocity vector
	// UWorld* World = GetWorld();
	// if (World != nullptr)
	// {
	// 	UKismetSystemLibrary::DrawDebugArrow(World, GetActorLocation(), GetActorLocation() + Velocity, 100.f, FColor::Green, 10.f, 1.f);		
	// }
	
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *FirstPoint.ToString())
}

void UUnitMovementComponent::AddNewPathPoint_Implementation(FVector NewPoint)
{
	PathPoints.Add(NewPoint);
}
