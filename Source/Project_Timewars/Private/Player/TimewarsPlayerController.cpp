// Fill out your copyright notice in the Description page of Project Settings.

#include "TimewarsPlayerController.h"



#include "DrawDebugHelpers.h"
#include "StrategyHelpers.h"
#include "StrategySelectionComponent.h"
#include "TimewarsSpectatorPawn.h"
#include "UnitAIController.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/GameModeBase.h"
#include "UI/MovementArrowActor.h"

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
    
    // Setup movement arrow animations     
    MovementArrowActor = GetWorld()->SpawnActor<AMovementArrowActor>(AMovementArrowActor::StaticClass(), FVector(0,0,0), FRotator(0,0,0));
}

void ATimewarsPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Right / Left click
    InputComponent->BindAction("MouseLeft", IE_Pressed, this, &ATimewarsPlayerController::StartSelection);
    InputComponent->BindAction("MouseLeft", IE_Released, this, &ATimewarsPlayerController::EndSelection);

    InputComponent->BindAction("MouseRight", IE_Pressed, this, &ATimewarsPlayerController::MouseRight);
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

void ATimewarsPlayerController::MouseRight()
{
    FVector2D MousePosition;
    StrategyHelpers::GetMousePosition(MousePosition, this);

    FVector MouseToWorld;
    StrategyHelpers::DeprojectPositionToWorld(MousePosition, MouseToWorld, this, ECollisionChannel::ECC_GameTraceChannel1);

    
    // Handle movement input
    for (auto a : SelectionComponent->GetCurrentSelectionControllers())
    {
        a->MouseRight(TimewarsPawn, MouseToWorld);
    }
    
    // Fire animation
    MovementArrowActor->PlayAnimationAtWorldPosition(MouseToWorld);
}