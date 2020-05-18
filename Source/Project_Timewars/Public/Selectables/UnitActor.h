// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectableActor.h"

#include "UnitActor.generated.h"

class UAnimationAsset;
class UAnimSequence;
/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API AUnitActor : public ASelectableActor
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
	
protected:
    void BeginPlay() override;

private:

};
