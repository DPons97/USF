// Fill out your copyright notice in the Description page of Project Settings.


#include "Project_Timewars/Public/StrategyHelpers.h"


#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

class UNavigationSystemV1;

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

FColor StrategyHelpers::GetTeamColor(ETeam::Type Team)
{
    switch (Team)
    {
    case ETeam::Neutral:
        return FColor::FromHex("C21F00FF") ;
    case ETeam::Zombie:
        return FColor::Red;
    case ETeam::Survivor:
        return FColor::Black;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Invalid team type"))
        return FColor::Black;
    }
}

FColor StrategyHelpers::GetTeamColor(ESurvivorColor::Type SurvivorColor)
{
    switch (SurvivorColor)
    {
    case ESurvivorColor::Brown:
        return FColor::FromHex("C21F00FF") ;
    case ESurvivorColor::Red:
        return FColor::Red;
    case ESurvivorColor::Blue:
        return FColor::Blue;
    case ESurvivorColor::Cyan:
        return FColor::Cyan;
    case ESurvivorColor::Green:
        return FColor::Green;
    case ESurvivorColor::Purple:
        return FColor::Purple;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Invalid team type"))
        return FColor::Black;
    }
}

void StrategyHelpers::GetActorsInSelectionRectangle(TSubclassOf<class AActor> ClassFilter, const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<AActor*>& OutActors, APlayerController* PC, float CameraZAngle)
{
    // Because this is a HUD function it is likely to get called each tick,
    // so make sure any previous contents of the out actor array have been cleared!
    OutActors.Reset();
    
    FVector2D P1(FirstPoint.X, FirstPoint.Y);
    FVector2D P2(SecondPoint.X, FirstPoint.Y);
    FVector2D P3(SecondPoint.X, SecondPoint.Y);
    FVector2D P4(FirstPoint.X, SecondPoint.Y);

    // Rotation
    FRotator RefRotation = FRotator(0, -CameraZAngle, 0);
    
    FVector WorldP1, WorldP2, WorldP3, WorldP4;
    DeprojectPositionToWorld(P1, WorldP1, PC, ECC_WorldStatic);
    DeprojectPositionToWorld(P2, WorldP2, PC, ECC_WorldStatic);
    DeprojectPositionToWorld(P3, WorldP3, PC, ECC_WorldStatic);
    DeprojectPositionToWorld(P4, WorldP4, PC, ECC_WorldStatic);

    // Packing into tranform
    FTransform RefTransform = FTransform(RefRotation, FVector::ZeroVector);

    FVector RotatedPoly[4] = {
        RefTransform.TransformVector(WorldP1),
        RefTransform.TransformVector(WorldP2),
        RefTransform.TransformVector(WorldP3),
        RefTransform.TransformVector(WorldP4)
    };

    // Translation
    TArray<float> X_i = {RotatedPoly[1].X, RotatedPoly[2].X, RotatedPoly[3].X, RotatedPoly[4].X};
    TArray<float> Y_i = {RotatedPoly[1].Y, RotatedPoly[2].Y, RotatedPoly[3].Y, RotatedPoly[4].Y};

    FVector RefTranslation = FVector(
        FMath::Abs(FMath::Min(X_i)),
        FMath::Abs(FMath::Min(Y_i)),
        0.f
    );
    RefTransform.SetTranslation(RefTranslation);
        
    // Frustum representation
    FVector Polygon[4] = {
        RefTransform.TransformPosition(WorldP1),
        RefTransform.TransformPosition(WorldP2),
        RefTransform.TransformPosition(WorldP3),
        RefTransform.TransformPosition(WorldP4)
    };

    // DEBUG
    //UE_LOG(LogTemp, Warning, TEXT("World_position = %f; Frustum_position = %f ==> Translation = %s"), WorldP1.X, Polygon[0].X, *RefTranslation.ToString())
    //DrawDebugSphere(PC->GetWorld(), Polygon[0], 10.f, 12, FColor::Red, true, 1.0f);
    //DrawDebugSphere(PC->GetWorld(), Polygon[1], 10.f, 12, FColor::Green, true, 1.0f);
    //DrawDebugSphere(PC->GetWorld(), Polygon[2], 10.f, 12, FColor::Blue, true, 1.0f);
    //DrawDebugSphere(PC->GetWorld(), Polygon[3], 10.f, 12, FColor::Cyan, true, 1.0f);
    
    for (TActorIterator<AActor> Itr(PC->GetWorld(), ClassFilter); Itr; ++Itr)
    {
        AActor* EachActor = *Itr;

        FVector ActorPosition = EachActor->GetActorLocation();
        ActorPosition = RefTransform.TransformPosition(ActorPosition);
        
        //Selection Box must enclose the Projected Actor Bounds
        if(IsPointInsidePoly(ActorPosition, Polygon, 4, PC, RefTransform))
        {
            //UE_LOG(LogTemp, Warning, TEXT("Actor inside point: %s"), *EachActor->GetName())
            OutActors.Add(EachActor);
        }
    }
}

/**
 *
 */
bool StrategyHelpers::IsPointInsidePoly(FVector Point, const FVector Polygon[], uint8 nVertices, APlayerController* PC, FTransform RefTransform) {
    FVector outIntersectionPoint;

    // Check vertical (Y) bounds
    const FVector HRayDirection = RefTransform.InverseTransformVector(FVector::RightVector);

    // Translate frustum vertical segments into planes
    FPlane VPlane1(Polygon[0], Polygon[3], FVector(Polygon[3].X, Polygon[3].Y, 1.f));
    FPlane VPlane2(Polygon[1], Polygon[2], FVector(Polygon[1].X, Polygon[1].Y, 1.f));

    // Compute horizontal distance from planes
    float HDist1 = FMath::Abs(Polygon[0].Y - Point.Y);
    float HDist2 = FMath::Abs(Polygon[2].Y - Point.Y);

    float HRayLength = 100 * FMath::Max(HDist1, HDist2);
    FVector HRayEnd = Point + HRayDirection * HRayLength;    

    bool IntersectsVPlane1 = FMath::SegmentPlaneIntersection(Point, HRayEnd, VPlane1, outIntersectionPoint);
    bool IntersectsVPlane2 = FMath::SegmentPlaneIntersection(Point, HRayEnd, VPlane2, outIntersectionPoint);

    bool InsideHorizonalBounds = (IntersectsVPlane1 || IntersectsVPlane2) && !(IntersectsVPlane1 && IntersectsVPlane2);

    //DrawDebugLine(PC->GetWorld(), Point, HRayEnd, FColor::Red, true, 15.f);
    //DrawDebugSolidPlane(PC->GetWorld(), VPlane1, Polygon[0], 200.f, FColor::Red, true, 15.f);
    //DrawDebugSolidPlane(PC->GetWorld(), VPlane2, Polygon[1], 200.f, FColor::Red, true, 15.f);

    // Check vertical (X) bounds
    const FVector VRayDirection = RefTransform.InverseTransformVector(FVector::ForwardVector);

    FPlane HPlane1(Polygon[0], Polygon[1], FVector(Polygon[0].X, Polygon[0].Y, 1.f));
    FPlane HPlane2(Polygon[2], Polygon[3], FVector(Polygon[2].X, Polygon[2].Y, 1.f));

    // Compute horizontal distance from planes
    float VDist1 = FMath::Abs(Polygon[0].X - Point.X);
    float VDist2 = FMath::Abs(Polygon[2].X - Point.X);

    float VRayLength = 100 * FMath::Max(VDist1, VDist2);
    FVector VRayEnd = Point + VRayDirection * VRayLength;    

    bool IntersectsHPlane1 = FMath::SegmentPlaneIntersection(Point, VRayEnd, HPlane1, outIntersectionPoint);
    bool IntersectsHPlane2 = FMath::SegmentPlaneIntersection(Point, VRayEnd, HPlane2, outIntersectionPoint);

    bool InsideVerticalBounds = (IntersectsHPlane1 || IntersectsHPlane2) && !(IntersectsHPlane1 && IntersectsHPlane2);

    //DrawDebugLine(PC->GetWorld(), Point, VRayEnd, FColor::Red, true, 15.f);
    //DrawDebugSolidPlane(PC->GetWorld(), HPlane1, Polygon[0], 200.f, FColor::Red, true, 15.f);
    //DrawDebugSolidPlane(PC->GetWorld(), HPlane2, Polygon[2], 200.f, FColor::Red, true, 15.f);
    
    return InsideHorizonalBounds && InsideVerticalBounds;
}

float StrategyHelpers::GetTerrainHeight(FVector2D Point, const UObject* Controller, bool bDrawDebugLines)
{
    UWorld* World = GEngine->GetWorldFromContextObject(Controller, EGetWorldErrorMode::Assert);
    if (World == nullptr) return 0.f;

    FVector StartLocation{ Point.X, Point.Y, 1000 };        // Raytrace starting point.
    FVector EndLocation{ Point.X, Point.Y, 0 };             // Raytrace end point.

    // Raytrace for overlapping actors.
    FHitResult HitResult;
    World->LineTraceSingleByObjectType(
        OUT HitResult,
        StartLocation,
        EndLocation,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
        FCollisionQueryParams()
    );

    // Draw debug line.
    if (bDrawDebugLines)
    {
        FColor LineColor;

        if (HitResult.GetActor()) LineColor = FColor::Red;
        else LineColor = FColor::Green;

        DrawDebugLine(
            World,
            StartLocation,
            EndLocation,
            LineColor,
            true,
            5.f,
            0.f,
            10.f
        );
    }

    // Return Z location.
    if (HitResult.GetActor()) return HitResult.ImpactPoint.Z;
    return 0;
}

