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
	// Receive command to move to a specific destination finding and following an optimal path
	UFUNCTION(Server, Reliable, WithValidation, Category="Movement")
    void CommandNavMoveTo(const FVector& Destination, bool DrawPath = false);

	// Receive command to start moving to a specific point
	UFUNCTION(Reliable, NetMulticast, Category="Movement")
    void StartMoveTo(const FVector& FirstPoint);

	// Receive command to stop current moving
	UFUNCTION(Server, Reliable, WithValidation, Category="Movement")
    void CommandNavStopMove();
	
	// Receive command to stop moving
	UFUNCTION(Reliable, NetMulticast, Category="Movement")
    void StopMoveTo();

	// Synchronize actor position between clients
	// As for now this is the simplest and a good enough solution to high lag and pkt losses.
	// Can be optimized in future using spline interpolation or other replication features.
	UFUNCTION(Reliable, NetMulticast, Category="Movement")
    void SyncLocationAndRotation(const FVector& ServerLocation, const FRotator ServerRotation);

	FVector GetCurrentDestination() const;

public:
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;
private:
	AUnitActor * Unit;

	float AcceptableRadius;

public:
	UFUNCTION(BlueprintCallable)
	float GetAcceptableRadius() const { return AcceptableRadius; }

	UFUNCTION(BlueprintCallable)
	void SetAcceptableRadius(float NewAcceptableRadius) { AcceptableRadius = NewAcceptableRadius; }

private:
	// Array populated with path points to be followed by movement engine
	TArray<FVector> PathPoints;
	
	TArray<FTimerHandle> LastPathPointsHandles;
	
	void ApplyRotation(float DeltaSeconds);
	
	void ApplyLocation(float DeltaSeconds);

	void ClearPathPoints();
};
