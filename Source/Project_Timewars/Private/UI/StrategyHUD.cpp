// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyHUD.h"

#include "Kismet/GameplayStatics.h"
#include "SelectableActor.h"
#include "Project_Timewars/Public/StrategyHelpers.h"

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
			a->SetActorPreSelected(false);
		}
		LastSelection.Empty();		
		GetActorsInSelectionRectangle<ASelectableActor>(
            SelectionStartPoint,
            SelectionEndPoint,
            LastSelection,
            true,
            false
        );

		for (auto a : LastSelection)
		{
			a->SetActorPreSelected(true);
		}
		
		// UE_LOG(LogTemp, Warning, TEXT("Selected actors: %d"), LastSelection.Num())
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
