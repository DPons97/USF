// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StrategyHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API AStrategyHUD : public AHUD
{
	GENERATED_BODY()

public:
	void StartSelection();
	
	void EndSelection();

	void DrawHUD() override;

	TArray<class ASelectablePawn*> GetCurrentSelection();

	UPROPERTY(EditDefaultsOnly)
	float SelectionBorderThickness = 1.f;

protected:
	AStrategyHUD();

	void BeginPlay() override;
private:
	void DrawSelectionBox();
	
	void DrawSelection();

	template<typename T>
	void ConfirmSelection(TArray<T*> SelectedUnits);
	
	bool isSelecting = false;

	// Player controller and owner of this HUD
	class ATimewarsPlayerController * PlayerController;
	
	class ATimewarsSpectatorPawn * SpectatorPawn;
	
	// Storing starting and ending selection points
	FVector2D SelectionStartPoint;

	FVector2D SelectionEndPoint;

	TArray<ASelectablePawn*> LastSelection;
};
