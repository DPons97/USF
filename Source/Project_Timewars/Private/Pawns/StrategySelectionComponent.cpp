// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/StrategySelectionComponent.h"

#include "DrawDebugHelpers.h"
#include "SelectableActor.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UStrategySelectionComponent::UStrategySelectionComponent()
{
    
}

void UStrategySelectionComponent::TickComponent(float DeltaTime,enum ELevelTick TickType,FActorComponentTickFunction * ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    
}

void UStrategySelectionComponent::SelectUnit()
{
    auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (!ensure(PlayerController != nullptr)) return;

    auto World = GetWorld();
    if (!ensure(World != nullptr)) return;

    float mouseX, mouseY;

    PlayerController->GetMousePosition(mouseX, mouseY);
    FVector2D mousePosition(mouseX, mouseY);
    
    // Linetrace mouse to world
    FHitResult HitResult;
    FCollisionQueryParams Params;
    if (!PlayerController->GetHitResultAtScreenPosition(mousePosition, ECollisionChannel::ECC_WorldDynamic, Params, HitResult))
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to trace raycast from mouse position to world"));
    }

    if (!HitResult.bBlockingHit)
    {
        return;
    }

    // DrawDebugSphere(World, HitResult.Location, MaxSelectionApprox, 12, FColor::Red, true, 15.0f);
    
    // Check overlapping actors to selection sphere
    TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
    traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
    
    UClass* selectableClass = ASelectableActor::StaticClass();
    TArray<AActor*> ignoreActors;
    TArray<AActor*> outActors;
        
    UKismetSystemLibrary::SphereOverlapActors(this, HitResult.Location, MaxSelectionApprox, traceObjectTypes, selectableClass, ignoreActors, outActors);

    if (outActors.Num() == 0)
    {
        ResetSelection();
    }

    MakeSelection(outActors);
}

void UStrategySelectionComponent::ResetSelection()
{
    for (auto actor : SelectedActors)
    {
        actor->SetActorSelected(false);
    }
}

void UStrategySelectionComponent::MakeSelection(TArray<AActor*> outActors)
{
    for (auto actor : outActors)
    {
        ASelectableActor* SelectableActor = Cast<ASelectableActor>(actor);
        SelectedActors.Add(SelectableActor);
        SelectableActor->SetActorSelected(true);
    }
}