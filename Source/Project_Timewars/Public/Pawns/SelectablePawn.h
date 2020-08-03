// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StrategySelectionInterface.h"
#include "GameFramework/Character.h"
#include "SelectablePawn.generated.h"


UCLASS()
class PROJECT_TIMEWARS_API ASelectablePawn :
    public ACharacter, public IStrategySelectionInterface
{
	GENERATED_BODY()
	
public:	
    // Sets default values for this actor's properties
    ASelectablePawn();

	virtual void SetActorSelected(bool isSelected) override;

	virtual void SetActorPreSelected(bool isPreSelected) override;

	class IStrategyCommandInterface* GetControllerInterface();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

	void ApplyAnimation(TSoftObjectPtr<class UAnimationAsset> newAnimation);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class UStaticMeshComponent* SelectionCircle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* PreSelectionCircle;

	// Actor mesh and animations
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class USkeletalMeshComponent* ActorSkeletalMesh;
};