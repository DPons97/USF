// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyHUD.h"

#include "BuildingActor.h"
#include "UnitActor.h"
#include "ResourceActor.h"
#include "StrategyHelpers.h"

AStrategyHUD::AStrategyHUD()
{
	
}

void AStrategyHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawSelection();
}

void AStrategyHUD::StartSelection()
{
	FVector2D MousePosition;
	if (!StrategyHelpers::GetMousePosition(MousePosition, GetOwningPlayerController())) return;

	SelectionStartPoint = MousePosition;
	isSelecting = true;
}

void AStrategyHUD::EndSelection()
{
	LastSelection.Empty();
	isSelecting = false;
}

TArray<ASelectableActor*> AStrategyHUD::GetCurrentSelection()
{
	return LastSelection;
}

void AStrategyHUD::DrawSelection()
{
	// Draw selection box
	if (isSelecting)
	{
		// Compute position
		if (!StrategyHelpers::GetMousePosition(SelectionEndPoint, GetOwningPlayerController())) return;
		
		DrawSelectionBox();
		for (auto a : LastSelection)
		{
			Cast<IStrategySelectionInterface>(a)->SetActorPreSelected(false);
		}
		
		LastSelection.Empty();

		// First, select units
		TArray<AUnitActor*> SelectedUnits;
		GetActorsInSelectionRectangle<AUnitActor>(
            SelectionStartPoint,
            SelectionEndPoint,
            SelectedUnits,
            true,
            false
        );

		if (SelectedUnits.Num() > 0)
		{
			ConfirmSelection<AUnitActor>(SelectedUnits);
			return;
		}

		// If no units inside selection rectangle, search for buildings
		TArray<ABuildingActor*> SelectedBuildings;
		GetActorsInSelectionRectangle<ABuildingActor>(
            SelectionStartPoint,
            SelectionEndPoint,
            SelectedBuildings,
            true,
            false
        );

		if (SelectedBuildings.Num() > 0)
		{
			ConfirmSelection<ABuildingActor>(SelectedBuildings);
			return;
		}

		// If no units nor buildings inside selection rectangle, try resources
		TArray<AResourceActor*> SelectedResources;
		GetActorsInSelectionRectangle<AResourceActor>(
            SelectionStartPoint,
            SelectionEndPoint,
            SelectedResources,
            true,
            false
        );

		if (SelectedResources.Num() > 0)
		{
			ConfirmSelection<AResourceActor>(SelectedResources);
		}
		
		// UE_LOG(LogTemp, Warning, TEXT("Selected actors: %d"), LastSelection.Num())
	}
}

template<typename T>
void AStrategyHUD::ConfirmSelection(TArray<T*> SelectedUnits)
{
	for (auto a : SelectedUnits)
	{
		LastSelection.Add(a);
		a->SetActorPreSelected(true);
	}
}

void AStrategyHUD::DrawSelectionBox()
{
	DrawLine(
        SelectionStartPoint.X,
        SelectionStartPoint.Y,
        SelectionEndPoint.X,
        SelectionStartPoint.Y,
        FLinearColor::Green,
        SelectionBorderThickness
    );
		
	DrawLine(
        SelectionEndPoint.X,
        SelectionStartPoint.Y,
        SelectionEndPoint.X,
        SelectionEndPoint.Y,
        FLinearColor::Green,
        SelectionBorderThickness
    );
		
	DrawLine(
        SelectionEndPoint.X,
        SelectionEndPoint.Y,
        SelectionStartPoint.X,
        SelectionEndPoint.Y,
        FLinearColor::Green,
        SelectionBorderThickness
    );
		
	DrawLine(
        SelectionStartPoint.X,
        SelectionEndPoint.Y,
        SelectionStartPoint.X,
        SelectionStartPoint.Y,
        FLinearColor::Green,
        SelectionBorderThickness
    );
}
