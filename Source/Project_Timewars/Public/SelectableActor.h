// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StrategyUtils.h"

#include "CoreMinimal.h"
#include "StrategyCommandInterface.h"
#include "StrategySelectionInterface.h"
#include "GameFramework/Pawn.h"

#include "SelectableActor.generated.h"

UCLASS()
class PROJECT_TIMEWARS_API ASelectableActor :
	public APawn, public IStrategySelectionInterface, public IStrategyCommandInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	explicit ASelectableActor();

	virtual void SetActorSelected(bool isSelected) override;

	virtual void SetActorPreSelected(bool isPreSelected) override;

	virtual void Move(FVector ClickPosition) PURE_VIRTUAL(ASelectableActor::LeftMouse, )

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ApplyAnimation(TSoftObjectPtr<class UAnimationAsset> newAnimation);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class USceneComponent* DefaultSceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class UStaticMeshComponent* SelectionCircle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* PreSelectionCircle;

	// Actor mesh and animations
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class USkeletalMeshComponent* ActorSkeletalMesh;
private:

};