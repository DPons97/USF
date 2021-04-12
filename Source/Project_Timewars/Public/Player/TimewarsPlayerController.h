// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "TimewarsSpectatorPawn.h"
#include "UnitAIAction.h"
#include "SelectablePawn.h"
#include "StrategyCommandInterface.h"
#include "GameFramework/PlayerController.h"
#include "TimewarsPlayerController.generated.h"

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
	UFUNCTION(Server, Reliable, WithValidation, Category="Input")
    void BeginGroupMovement(const TArray<AUnitActor*>& Selection, FVector Destination);

	/**
	* 	Returns the ID of the group containing this selection of units.
	* 	@return Group containing entire selection. nullptr if no such common group exists.
	*/
	USelectableGroup* GetUnitsGroup(TArray<AUnitActor*> Selection) const;

	UFUNCTION(BlueprintCallable)
    ATimewarsSpectatorPawn* GetTimewarsPawn() const { return TimewarsPawn; }

	template<typename UserClass>
    void OrderSimpleMovement(ASelectablePawn* PawnToMove, FVector Destination, UserClass* DelegateObj, bool bOverridePreviousMovements = false,
    	typename FActionStarted::TUObjectMethodDelegate<UserClass>::FMethodPtr OnStartDelegate = nullptr,
    	typename FActionEnded::TUObjectMethodDelegate<UserClass>::FMethodPtr OnEndDelegate = nullptr)
	{		
		UUnitAIAction* NewAction = UUnitAIAction::CreateAction(TimewarsPawn, EUnitActionType::MoveTo, Destination,
        nullptr, bOverridePreviousMovements);
		
		if (OnStartDelegate) NewAction->BindOnActionStarted(DelegateObj, OnStartDelegate);
		if (OnEndDelegate) NewAction->BindOnActionEnded(DelegateObj, OnEndDelegate);
    
		// Handle movement input
		PawnToMove->GetControllerInterface()->EnqueueAction(NewAction);
	}

	template<typename UserClass>
    void OrderPathMovement(ASelectablePawn* PawnToMove, const TArray<FVector>& Path, UserClass* DelegateObj, bool bOverridePreviousMovements = false,
        typename FActionStarted::TUObjectMethodDelegate<UserClass>::FMethodPtr OnStartDelegate = nullptr,
        typename FActionEnded::TUObjectMethodDelegate<UserClass>::FMethodPtr OnEndDelegate = nullptr)
	{
		if (Path.Num() == 0) return;
		
		UUnitAIAction* NewAction = UUnitAIAction::CreateAction(
            TimewarsPawn, EUnitActionType::MoveTo, Path[0], nullptr, bOverridePreviousMovements);

		if (OnStartDelegate) NewAction->BindOnActionStarted(DelegateObj, OnStartDelegate);
		if (OnEndDelegate) NewAction->BindOnActionEnded(DelegateObj, OnEndDelegate);
    
		// Handle movement input
		PawnToMove->GetControllerInterface()->EnqueueAction(NewAction);
		for (int i = 1; i < Path.Num(); i++)
		{
			NewAction = UUnitAIAction::CreateAction(
                TimewarsPawn,
                EUnitActionType::MoveTo,
                Path[i]);
        
			PawnToMove->GetControllerInterface()->EnqueueAction(NewAction);   
		}    
	}

	TArray<AUnitActor*> GetSelectionAsActors();
	
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
	
	void SetUnitGroup(AUnitActor* u, USelectableGroup* group);
};
