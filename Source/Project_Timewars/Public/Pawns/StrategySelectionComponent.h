// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectableActor.h"
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
	bool GetMousePosition(FVector2D &outMousePosition);
	
	bool DeprojectPositionToWorld(FVector2D position, FVector &outWorldPosition);

	TArray<ASelectableActor*> MakeSingleSelection();
	
	TArray<ASelectableActor*> MakeMultipleSelection();

	void ResetSelection();

	void SelectActors(TArray<ASelectableActor*> selectedActors);

	APlayerController* PlayerController;

	AStrategyHUD* HUD;
	
	TArray<ASelectableActor*> SelectedActors; 

	bool isSelecting;

	// Storing starting and ending selection points
	FVector2D SelectionStartPoint;

	FVector2D SelectionEndPoint;
};
