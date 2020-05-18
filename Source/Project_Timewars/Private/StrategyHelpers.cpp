// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_Timewars/Public/StrategyHelpers.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

StrategyHelpers::StrategyHelpers()
{
}

StrategyHelpers::~StrategyHelpers()
{
}

bool StrategyHelpers::GetMousePosition(FVector2D &outMousePosition, APlayerController* PlayerController)
{
    if (PlayerController == nullptr) return false;

    float mouseX, mouseY;
    if (!PlayerController->GetMousePosition(mouseX, mouseY))
    {
        return false;
    }

    outMousePosition = FVector2D(mouseX, mouseY);
    return true;
}

bool StrategyHelpers::DeprojectPositionToWorld(FVector2D position, FVector &outWorldPosition, APlayerController* PlayerController, ECollisionChannel CollisionChannel)
{
    if (!ensure(PlayerController != nullptr)) return false;

    // Ray trace mouse to world
    FHitResult HitResult;
    FCollisionQueryParams Params;
    if (!PlayerController->GetHitResultAtScreenPosition(position, CollisionChannel, Params, HitResult))
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to trace raycast from mouse position to world"));
        return false;
    }

    if (!HitResult.bBlockingHit)
    {
        return false;
    }

    outWorldPosition = HitResult.Location;
    return true;
}