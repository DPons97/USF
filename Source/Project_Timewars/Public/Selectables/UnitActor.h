// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StrategyCommandInterface.h"
#include "Pawns/SelectablePawn.h"
#include "UnitActor.generated.h"

class UAnimationAsset;
class UAnimSequence;
/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API AUnitActor : public ASelectablePawn, public IStrategyCommandInterface
{
	GENERATED_BODY()

public:
	AUnitActor();

	/* Strategy interface implementations */
	void Move(FVector ClickPosition) override;

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
	UAnimationAsset* GetAnimation(TSoftObjectPtr<UAnimationAsset> Animation);
	
protected:
    void BeginPlay() override;

private:

};
