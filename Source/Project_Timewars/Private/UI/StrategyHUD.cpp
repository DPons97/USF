// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyHUD.h"

#include "Kismet/GameplayStatics.h"
#include "SelectableActor.h"




void AStrategyHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw selection box
	if (isSelecting)
	{
		// Compute position
		if (!GetMousePosition(SelectionEndPoint)) return;
		
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

void AStrategyHUD::StartSelection()
{
	FVector2D MousePosition;
	if (!GetMousePosition(MousePosition)) return;

	SelectionStartPoint = MousePosition;
	isSelecting = true;
}

void AStrategyHUD::EndSelection()
{
	LastSelection.Empty();
	isSelecting = false;
}

bool AStrategyHUD::GetMousePosition(FVector2D& MousePosition)
{
	const auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!ensure(PlayerController != nullptr)) return false;

	float mouseX, mouseY;
	if (!PlayerController->GetMousePosition(mouseX, mouseY))
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to get mouse position"));
		return false;
	}
	
	MousePosition = FVector2D(mouseX, mouseY);
	return true;
}

TArray<ASelectableActor*> AStrategyHUD::GetCurrentSelection()
{
	return LastSelection;
}

bool AStrategyHUD::DrawSelectionBox()
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

	return false;
}
