// Fill out your copyright notice in the Description page of Project Settings.

#include "TimewarsPlayerController.h"

#include "StrategySelectionComponent.h"
#include "TimewarsSpectatorPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/GameModeBase.h"

ATimewarsPlayerController::ATimewarsPlayerController()
{    
    bShowMouseCursor = true;
    bEnableClickEvents = true;

    // Init selection handler
    SelectionComponent = CreateDefaultSubobject<UStrategySelectionComponent>(TEXT("StrategySelectionComponent"));
}

void ATimewarsPlayerController::BeginPlay()
{
    Super::BeginPlay();

    TimewarsPawn = Cast<ATimewarsSpectatorPawn>(GetPawn());
}

void ATimewarsPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Right / Left click
    InputComponent->BindAction("MouseLeft", IE_Pressed, this, &ATimewarsPlayerController::StartSelection);
    InputComponent->BindAction("MouseLeft", IE_Released, this, &ATimewarsPlayerController::EndSelection);

    InputComponent->BindAction("MouseRight", IE_Pressed, this, &ATimewarsPlayerController::GiveMovementOrder);
}

void ATimewarsPlayerController::EndSelection()
{
    SelectionComponent->EndSelection();
    TimewarsPawn->EndSelection();
}

void ATimewarsPlayerController::StartSelection()
{
    SelectionComponent->StartSelection();
    TimewarsPawn->StartSelection();
}

void ATimewarsPlayerController::GiveMovementOrder()
{    
    // Handle movement input

    // Fire animation

    TimewarsPawn->FireMovementPing();
}