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

	// Receive command to move to a specific destination finding and following an optimal path
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
    void MoveTo(const FVector& Destination, float AcceptableRadius) const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	void StopMoving() const;

	UFUNCTION(BlueprintCallable)
    FVector GetCurrentDestination() const;
protected:
    void BeginPlay() override;

	void Tick(float DeltaSeconds) override;

private:
	UUnitMovementComponent* UnitMovementComponent;
	
};
