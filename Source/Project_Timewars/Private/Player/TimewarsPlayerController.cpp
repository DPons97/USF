// Fill out your copyright notice in the Description page of Project Settings.

#include "TimewarsPlayerController.h"

#include "DrawDebugHelpers.h"
#include "SelectableGroup.h"
#include "StrategyHelpers.h"
#include "StrategySelectionComponent.h"
#include "TimewarsSpectatorPawn.h"
#include "UnitActor.h"
#include "UnitAIController.h"
#include "UI/MovementArrowActor.h"

ATimewarsPlayerController::ATimewarsPlayerController()
{    
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    SetActorTickEnabled(false); 
    
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

void ATimewarsPlayerController::MouseRight()
{
    // todo Handle other right clicks

    // Movement handling
    FVector2D MousePosition;
    StrategyHelpers::GetMousePosition(MousePosition, this);

    FVector MouseToWorld;
    StrategyHelpers::DeprojectPositionToWorld(MousePosition, MouseToWorld, this, ECollisionChannel::ECC_GameTraceChannel1);

    // todo differentiate on type of selection (buildings, units, resources)

    // Unit actors are only actors able to move
    BeginGroupMovement(GetSelectionAsActors(), MouseToWorld);    
    
    // Fire animation
    MovementArrowActor->PlayAnimationAtWorldPosition(MouseToWorld);
}

void ATimewarsPlayerController::BeginGroupMovement_Implementation(const TArray<AUnitActor*>& Selection,
                                                                  FVector Destination)
{
    if (Selection.Num() == 0) return;
    
    if (Selection.Num() == 1)
    {
        SetUnitGroup(Selection[0], nullptr);
        Selection[0]->ResetBaseSpeed();
        
        // Only one unit selected. Simple movement to location
        OrderSimpleMovement(Selection[0], Destination, this, true);
    }
    else
    {
        // Multiple selection. Use groups and formations
        USelectableGroup* SelGroup = GetUnitsGroup(Selection);
        if (SelGroup != nullptr)
        {
            // Give movement order to group
            SelGroup->GiveMovementOrder(GetWorld(), Destination);
        }
        else
        {
            // Create new group and give movement order
            USelectableGroup* NewGroup = NewObject<USelectableGroup>();
            NewGroup->SetFormationID(0);                // todo At the moment there is no dynamic formation choice. 
            NewGroup->AddUnits(Selection);
            NewGroup->SetType(EGroupType::Units);        // todo At the moment there is only one type of unit (i.e. no hybrid groups can exist)

            // Update existing groups and units with new groups associations
            for (auto a : Selection)
            {
                SetUnitGroup(a, NewGroup);
            }
            
            NewGroup->GiveMovementOrder(GetWorld(), Destination);
        }
    }
}

bool ATimewarsPlayerController::BeginGroupMovement_Validate(const TArray<AUnitActor*>& Selection,
    FVector Destination)
{
    // todo Check all units belong to this controller + additional validation
    return true;
}

USelectableGroup* ATimewarsPlayerController::GetUnitsGroup(TArray<AUnitActor*> Selection) const
{
    USelectableGroup* ExistingGroup = Selection[0]->GetGroup();
    if (!ExistingGroup) return nullptr;
    
    for (auto Unit : Selection)
    {
        const USelectableGroup* UnitGroup = Unit->GetGroup(); 
            
        if (UnitGroup != ExistingGroup)
        {
            // This unit has a different group than the others, or no group
            return nullptr;
        }
    }
    
    return Selection.Num() == ExistingGroup->GetNumberUnits() ? ExistingGroup : nullptr;
}

TArray<AUnitActor*> ATimewarsPlayerController::GetSelectionAsActors()
{
    TArray<ASelectablePawn*> Selection = SelectionComponent->GetCurrentSelection();
    TArray<AUnitActor*> UnitsSel;

    for (auto u : Selection)
    {
        UnitsSel.Add(Cast<AUnitActor>(u));
    }
    return UnitsSel;
}

void ATimewarsPlayerController::SetUnitGroup(AUnitActor* u, USelectableGroup* group)
{
    USelectableGroup* UnitGroup = u->GetGroup();

    if (UnitGroup)
    {
        UnitGroup->RemoveUnit(u);
    }
                
    u->SetGroup(group);
}
