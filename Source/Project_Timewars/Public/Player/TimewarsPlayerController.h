// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimewarsSpectatorPawn.h"
#include "GameFramework/PlayerController.h"
#include "TimewarsPlayerController.generated.h"

class ASelectablePawn;
class USelectableGroup;

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

	/**
	* 	Generate movement for a given selection of pawns.
	* 	For performance purposes, this method will perform the cast to AUnitActor internally exploiting an already
	* 	existing for loop.
	* 	Therefore it's important that the Selection only contains AUnitActors.
	*/
	UFUNCTION(Client, Reliable, Category="Input")
    void BeginGroupMovement(const TArray<ASelectablePawn*>& Selection, FVector Destination);

	UFUNCTION(Server, Reliable, WithValidation, Category="Input")
    void OrderSimpleMovement(ASelectablePawn* PawnToMove, FVector Destination, bool bOverridePreviousMovements);

	UFUNCTION(Server, Reliable, WithValidation, Category="Input")
    void OrderPathMovement(ASelectablePawn* PawnToMove, const TArray<FVector>& Path, bool bOverridePreviousMovements);

	/**
	* 	Returns the ID of the group containing this selection of units.
	* 	@return Group containing entire selection. nullptr if no such common group exists.
	*/
	USelectableGroup* GetUnitsGroup(TArray<ASelectablePawn*> Selection) const;
	
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

public:
	UFUNCTION(BlueprintCallable)
	ATimewarsSpectatorPawn* GetTimewarsPawn() const { return TimewarsPawn; }
};
