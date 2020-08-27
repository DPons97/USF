// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitMovementComponent.h"

#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "UnitAIController.h"

UUnitMovementComponent::UUnitMovementComponent()
{
	SetIsReplicatedByDefault(true);
}

void UUnitMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UnitOwner = Cast<AUnitActor>(GetOwner());
	ensure(UnitOwner != nullptr);
}

void UUnitMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Make sure that everything is still valid, and that we are allowed to move.
	if (!UnitOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	PerformMovement(DeltaTime);
}

void UUnitMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	if (MoveVelocity.SizeSquared() < KINDA_SMALL_NUMBER)
	{
		return;
	}
	
	Velocity = MoveVelocity;
	bHasRequestedVelocity = true;

	if (UnitOwner != nullptr && UnitOwner->HasActorBegunPlay()) SynchronizeClients(UnitOwner->GetActorTransform(), Velocity, bHasRequestedVelocity);
}

bool UUnitMovementComponent::CanStartPathFollowing() const
{
	return Super::CanStartPathFollowing();
}

void UUnitMovementComponent::StopActiveMovement()
{
	Super::StopActiveMovement();

	bHasRequestedVelocity = false;
	Velocity = FVector::ZeroVector;

	if (UnitOwner != nullptr && UnitOwner->HasActorBegunPlay()) SynchronizeClients(UnitOwner->GetActorTransform(), Velocity, bHasRequestedVelocity);
}

FRotator UUnitMovementComponent::ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime,
	FRotator& DeltaRotation) const
{
	// AI path following request can orient us in that direction (it's effectively an acceleration)
	if (bHasRequestedVelocity && Velocity.SizeSquared() > KINDA_SMALL_NUMBER)
	{
		return Velocity.GetSafeNormal().Rotation();
	}

	// Don't change rotation if there is no acceleration.
	return CurrentRotation;
}

void UUnitMovementComponent::PerformMovement(float DeltaTime)
{	
	if (Velocity == FVector::ZeroVector || !bHasRequestedVelocity)
	{
		return;
	}

	const FVector DesiredMovementThisFrame = Velocity.GetClampedToMaxSize(1.0f) * DeltaTime * (UnitOwner->ActorData.Speed * 100 / 3.6);
	
	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		// If we bumped into something, try to slide along it
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}

	ApplyRotation(DeltaTime);		
}

void UUnitMovementComponent::SynchronizeClients_Implementation(FTransform ServerTransform, FVector ServerVelocity, bool ServerRequestedVelocity)
{
	UnitOwner->SetActorTransform(ServerTransform);
	Velocity = ServerVelocity;
	bHasRequestedVelocity = ServerRequestedVelocity;
}

void UUnitMovementComponent::ApplyRotation(float DeltaTime)
{
	const FRotator CurrentRotation = UpdatedComponent->GetComponentRotation(); // Normalized
	CurrentRotation.DiagnosticCheckNaN(TEXT("UUnitMovementComponent::ApplyRotation(): CurrentRotation"));

	FRotator DeltaRot = GetDeltaRotation(DeltaTime);
	DeltaRot.DiagnosticCheckNaN(TEXT("UUnitMovementComponent::ApplyRotation(): GetDeltaRotation"));
	
	DesiredRotation = ComputeOrientToMovementRotation(CurrentRotation, DeltaTime, DeltaRot);
	
	// Accumulate a desired new rotation.
	const float AngleTolerance = 1e-3f;

	if (!CurrentRotation.Equals(DesiredRotation, AngleTolerance))
	{
		// PITCH
		if (!FMath::IsNearlyEqual(CurrentRotation.Pitch, DesiredRotation.Pitch, AngleTolerance))
		{
			DesiredRotation.Pitch = FMath::FixedTurn(CurrentRotation.Pitch, DesiredRotation.Pitch, DeltaRot.Pitch);
		}

		// YAW
		if (!FMath::IsNearlyEqual(CurrentRotation.Yaw, DesiredRotation.Yaw, AngleTolerance))
		{
			DesiredRotation.Yaw = FMath::FixedTurn(CurrentRotation.Yaw, DesiredRotation.Yaw, DeltaRot.Yaw);
		}

		// ROLL
		if (!FMath::IsNearlyEqual(CurrentRotation.Roll, DesiredRotation.Roll, AngleTolerance))
		{
			DesiredRotation.Roll = FMath::FixedTurn(CurrentRotation.Roll, DesiredRotation.Roll, DeltaRot.Roll);
		}

		// Set the new rotation.
		DesiredRotation.DiagnosticCheckNaN(TEXT("UUnitMovementComponent::ApplyRotation(): DesiredRotation"));
		MoveUpdatedComponent( FVector::ZeroVector, DesiredRotation, /*bSweep*/ false );
	}
}

float GetAxisDeltaRotation(float InAxisRotationRate, float DeltaTime)
{
	return (InAxisRotationRate >= 0.f) ? (InAxisRotationRate * DeltaTime) : 360.f;
}

FRotator UUnitMovementComponent::GetDeltaRotation(float DeltaTime) const
{
	const FRotator RotationRate = UnitOwner->ActorData.TurningSpeed * (180/PI);
	return FRotator(GetAxisDeltaRotation(RotationRate.Pitch, DeltaTime), GetAxisDeltaRotation(RotationRate.Yaw, DeltaTime), GetAxisDeltaRotation(RotationRate.Roll, DeltaTime));
}