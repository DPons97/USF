// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "MovementArrowActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API AMovementArrowActor : public ASkeletalMeshActor
{
	GENERATED_BODY()


public:
	AMovementArrowActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Selection)
	class ASkeletalMeshActor* MovementArrowSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Selection)
	class UAnimSequence* MovementArrowAnimSequence;

	void PlayAnimationAtMousePosition();

};