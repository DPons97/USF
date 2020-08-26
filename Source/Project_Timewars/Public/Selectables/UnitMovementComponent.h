// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitActor.h"
#include "GameFramework/PawnMovementComponent.h"
#include "UnitMovementComponent.generated.h"

class AStrategyAIController;
/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API UUnitMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	//BEGIN UMovementComponent Interface
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void StopActiveMovement() override;
	//END UMovementComponent Interface

	//BEGIN UNavMovementComponent Interface
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
	virtual bool CanStartPathFollowing() const override;
	//END UNaVMovementComponent Interface

	/**
	* Compute a target rotation based on current movement. Used by ApplyRotation().
	* Default implementation targets a rotation based on Acceleration.
	*
	* @param CurrentRotation	- Current rotation of the Character
	* @param DeltaTime		- Time slice for this movement
	* @param DeltaRotation	- Proposed rotation change based simply on DeltaTime * RotationRate
	*
	* @return The target rotation given current movement.
	*/
	virtual FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const;

	/** Returns how far to rotate character during the time interval DeltaTime. */
	FRotator GetDeltaRotation(float DeltaTime) const;
	
	/** Perform rotation over deltaTime */
	virtual void ApplyRotation(float DeltaTime);

	/** Perform actual movement */
	void PerformMovement(float DeltaTime);

	/** Replicate the next movement on the client */
	UFUNCTION(Reliable, NetMulticast, Category="Movement")
	void SynchronizeClients(FTransform ServerTransform, FVector ServerVelocity, bool ServerRequestedVelocity);

private:
	AUnitActor * UnitOwner;

	/** Was velocity requested by path following? */
	UPROPERTY(Transient)
	bool bHasRequestedVelocity;

	UPROPERTY(Transient)
	FRotator DesiredRotation;
};
