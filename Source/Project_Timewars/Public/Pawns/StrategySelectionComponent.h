// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StrategyHUD.h"
#include "Components/ActorComponent.h"
#include "StrategySelectionComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API UStrategySelectionComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	UStrategySelectionComponent();

	TArray<class IStrategyCommandInterface*> GetCurrentSelectionControllers();
	
    void EndSelection();

	void StartSelection();

protected:
	void BeginPlay() override;
	
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction);

	/*
     * Maximum approximation to select an actor
     */
    UPROPERTY(EditDefaultsOnly)
    float MaxSelectionApprox = 30.f;
    
private:
	TArray<class ASelectablePawn*> MakeSingleSelection();
	
	TArray<ASelectablePawn*> MakeMultipleSelection();

	void ResetSelection();

	void SelectActors(TArray<ASelectablePawn*> selectedActors);

	APlayerController* PlayerController;

	AStrategyHUD* HUD;

	/* Array of last selection. Actors in this array always implement IStrategySelectionInterface */
	TArray<ASelectablePawn*> SelectedActors;

	bool isSelecting;

	// Storing starting and ending selection points
	FVector2D SelectionStartPoint;

	FVector2D SelectionEndPoint;
};
