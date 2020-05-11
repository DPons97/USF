// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectableActor.h"
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

    void SelectUnit();
    
protected:
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction);
	
	void ResetSelection();

	void MakeSelection(TArray<AActor*> outActors);

	/*
     * Maximum approximation to select an actor
     */
    UPROPERTY(EditDefaultsOnly)
    float MaxSelectionApprox = 30.f;
    
private:
	TArray<ASelectableActor*> SelectedActors; 
	
    
};
