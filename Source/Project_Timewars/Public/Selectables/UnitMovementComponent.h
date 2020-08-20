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
    void CommandNavMoveTo(const FVector& Destination);

	// Receive command to start moving to a specific point
	UFUNCTION(Reliable, NetMulticast, Category="Movement")
    void StartMoveTo(const FVector& FirstPoint);

protected:
	UFUNCTION(Reliable, NetMulticast, Category = "Movement")
    void AddNewPathPoint(FVector NewPoint);

public:
	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	AUnitActor * Unit;
	
	FVector Velocity;

	// Array populated with path points to be followed by movement engine
	TArray<FVector> PathPoints;

	TArray<FTimerHandle> LastPathPointsHandles;
	
	void ApplyRotation(float DeltaSeconds);
	void ApplyLocation(float DeltaSeconds);
};
