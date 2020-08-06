// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimewarsSpectatorPawn.h"
#include "GameFramework/PlayerController.h"
#include "TimewarsPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API ATimewarsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATimewarsPlayerController();
	
protected:
	virtual void BeginPlay() override;
	
	void SetupInputComponent() override;

	// Selection component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
    class UStrategySelectionComponent* SelectionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Selection)
    class AMovementArrowActor* MovementArrowActor;
private:
	void EndSelection();

	void StartSelection();
	
	void MouseRight();
	
	ATimewarsSpectatorPawn* TimewarsPawn;
};
