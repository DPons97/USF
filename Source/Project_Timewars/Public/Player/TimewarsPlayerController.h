// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimewarsSpectatorPawn.h"
#include "GameFramework/PlayerController.h"
#include "TimewarsPlayerController.generated.h"

class ASelectablePawn;
/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API ATimewarsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATimewarsPlayerController();

	virtual void OnRep_Pawn() override;
	
protected:
	virtual void BeginPlay() override;
	
	void SetupInputComponent() override;

	// Selection component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
    class UStrategySelectionComponent* SelectionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Selection)
    class AMovementArrowActor* MovementArrowActor;

	UPROPERTY(BlueprintReadWrite)
	ATimewarsSpectatorPawn* TimewarsPawn;

	virtual void OnPossess(APawn* aPawn) override;
private:
	void EndSelection();

	void StartSelection();

	void MouseRight();

	UFUNCTION(Server, Reliable, WithValidation, Category="Input")
	void GiveMovementOrder(const TArray<ASelectablePawn*>& Selection, FVector Destination);

public:
	UFUNCTION(BlueprintCallable)
	ATimewarsSpectatorPawn* GetTimewarsPawn() const { return TimewarsPawn; }
};
