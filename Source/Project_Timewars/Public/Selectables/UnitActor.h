// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/SelectablePawn.h"
#include "UnitActor.generated.h"

class UAnimationAsset;
class UAnimSequence;
class UUnitMovementComponent;
class AStrategyAIController;

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API AUnitActor : public ASelectablePawn
{
	GENERATED_BODY()

public:
	AUnitActor(const FObjectInitializer& ObjectInitializer);

	// Actor meshes and animations
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSoftObjectPtr<UAnimationAsset> IdleAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSoftObjectPtr<UAnimationAsset> WalkingAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSoftObjectPtr<UAnimationAsset> RunningAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSoftObjectPtr<UAnimationAsset> AttackingAnimation;

	UFUNCTION(BlueprintCallable)
	void SetCurrentTask(enum EUnitTask NewTask) { CurrentTask = NewTask; }

	UFUNCTION(BlueprintCallable)
	EUnitTask GetCurrentTask() const { return CurrentTask; }
	
	// Receive command to move to a specific destination finding and following an optimal path
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category="Movement")
    void CommandNavMoveTo(const FVector& Destination);

	// Receive command to start moving to a specific point
	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category="Movement")
    void StartMoveTo(const FVector& FirstPoint);

protected:
    void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Reliable, NetMulticast, Category = "Movement")
	void AddNewPathPoint(FVector NewPoint);

private:
	EUnitTask CurrentTask;

	FVector Velocity;

	// Array populated with path points to be followed by movement engine
	TArray<FVector> PathPoints;

	TArray<FTimerHandle> LastPathPointsHandles;

	UUnitMovementComponent* UnitMovementComponent;
};
