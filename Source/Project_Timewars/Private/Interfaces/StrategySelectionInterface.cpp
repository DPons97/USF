// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategySelectionInterface.h"

// Add default functionality here for any IStrategySelectionInterface functions that are not pure virtual.
void IStrategySelectionInterface::SetActorSelected(bool isSelected)
{
	Selected = isSelected;
}

bool IStrategySelectionInterface::IsSelected() const
{
	return Selected;
}