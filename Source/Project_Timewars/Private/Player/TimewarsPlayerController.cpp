// Fill out your copyright notice in the Description page of Project Settings.

#include "TimewarsPlayerController.h"



#include "DrawDebugHelpers.h"
#include "StrategyHelpers.h"
#include "StrategySelectionComponent.h"
#include "TimewarsSpectatorPawn.h"
#include "UnitAIController.h"
#include "Components/StaticMeshComponent.h"
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

void ATimewarsPlayerController::OnPossess(APawn* aPawn)
{
    // OnPossess method only fires on the server
    Super::OnPossess(aPawn);
    
    TimewarsPawn = Cast<ATimewarsSpectatorPawn>(aPawn);
}

void ATimewarsPlayerController::OnRep_Pawn()
{
    TimewarsPawn = Cast<ATimewarsSpectatorPawn>(GetPawn());
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

void ATimewarsPlayerController::GiveMovementOrder_Implementation(const TArray<ASelectablePawn*>& Selection, FVector Destination)
{
    // Handle movement input
    for (auto a : SelectionComponent->GetCurrentSelectionControllers(Selection))
    {
        a->MouseRight(TimewarsPawn, Destination);
    }
}

bool ATimewarsPlayerController::GiveMovementOrder_Validate(const TArray<ASelectablePawn*>& Selection, FVector Destination)
{
    // todo validation
    return true;
}

void ATimewarsPlayerController::MouseRight()
{
    // todo Handle other right clicks

    // Movement handling
    FVector2D MousePosition;
    StrategyHelpers::GetMousePosition(MousePosition, this);

    FVector MouseToWorld;
    StrategyHelpers::DeprojectPositionToWorld(MousePosition, MouseToWorld, this, ECollisionChannel::ECC_GameTraceChannel1);

    GiveMovementOrder(SelectionComponent->GetCurrentSelection(), MouseToWorld);    
    
    // Fire animation
    MovementArrowActor->PlayAnimationAtWorldPosition(MouseToWorld);
}
