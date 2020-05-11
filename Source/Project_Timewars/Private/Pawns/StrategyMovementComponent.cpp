// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/StrategyMovementComponent.h"


#include "TimewarsSpectatorPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"

UStrategyMovementComponent::UStrategyMovementComponent()
{
    // set defaults
    CameraXYLimit = FVector2D(25000.f, 25000.f);
    CameraHeight = 1000.f;                    
    CameraHeightMin = 500.f;                // 100 for debugging
    CameraHeightMax = 4000.f;

    CameraRadius = 2000.f;                    
    CameraRadiusMin = 1000.f;                // 100 for debugging
    CameraRadiusMax = 5000.f;                

    CameraZAngle = 0.f;                        // yaw

    CameraHeightAngle = 50.f;                // pitch
    CameraHeightAngleMin = 15.f;
    CameraHeightAngleMax = 60.f;

    CameraZoomSpeed = 200.f;                // wheel
    CameraRotationSpeed = 10.f;                // wheel + ctrl
    CameraMovementSpeed = 3000.f;            // in all directions

    CameraScrollBoundary = 25.f;            // screen edge width

    bCanMoveCamera = true;
}


void UStrategyMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // mouse position and screen size
    FVector2D MousePosition;
    FVector2D ViewportSize;

    UWorld* World = GetWorld();
    if (!ensure(World!=nullptr)) return;
    
    UGameViewportClient* GameViewport = World->GetGameViewport();

    // it is always nullptr on dedicated server
    if (GameViewport == nullptr) return;
    GameViewport->GetViewportSize(ViewportSize);

    // if viewport is focused, contains the mouse, and camera movement is allowed
    if (GameViewport->IsFocused(GameViewport->Viewport)
        && GameViewport->GetMousePosition(MousePosition) && bCanMoveCamera)
    {
        //-------------------
        // movement direction by mouse at screen edge

        if (MousePosition.X < CameraScrollBoundary)
        {
            MoveRightValue = -1.0f;
        }
        else if (ViewportSize.X - MousePosition.X < CameraScrollBoundary)
        {
            MoveRightValue = 1.0f;
        }

        if (MousePosition.Y < CameraScrollBoundary)
        {
            MoveForwardValue = 1.0f;
        }
        else if (ViewportSize.Y - MousePosition.Y < CameraScrollBoundary)
        {
            MoveForwardValue = -1.0f;
        }

        //-------------------
        // tweak camera actor position

        FVector ActualLocation = GetActorLocation();
        FVector ActualMovement = FVector::ZeroVector;        

        // horizontal movement
        if (RotateValue == 0.f)
        {
            ActualMovement += MoveCameraForward(MoveForwardValue * FastMoveValue * DeltaTime);
            ActualMovement += MoveCameraRight(MoveRightValue * FastMoveValue * DeltaTime);
        }
        ActualLocation += ActualMovement;

        // vertical movement
        CameraHeight += MoveCameraUp(MoveUpValue * FastMoveValue * DeltaTime);
        CameraHeight = FMath::Clamp(CameraHeight, CameraHeightMin, CameraHeightMax);

        // adjust actor height to surface
        float TerrainSurfaceZ = GetLandTerrainSurfaceAtCoord(ActualLocation.X, ActualLocation.Y);
        ActualLocation.Z = TerrainSurfaceZ + CameraHeight;

        // limit movement area
        ActualLocation.X = FMath::Clamp(ActualLocation.X, -CameraXYLimit.X, CameraXYLimit.X);
        ActualLocation.Y = FMath::Clamp(ActualLocation.Y, -CameraXYLimit.Y, CameraXYLimit.Y);

        // move actor
        AActor* Owner = GetOwner();
        if (Owner == nullptr) return;
        
        Owner->SetActorLocation(ActualLocation);

        //-------------------
        // tweak camera component relative transform

        // set rotation parameters
        if (RotateValue != 0.f)
        {
            TurnCameraUp(MoveForwardValue * FastMoveValue * DeltaTime);
            TurnCameraRight(MoveRightValue * FastMoveValue * DeltaTime);
        }

        // set zoom distance
        ZoomCameraIn(ZoomInValue * FastMoveValue * DeltaTime);

        // adjust camera component relative location and rotation
        RepositionCamera();

        //-------------------
        // debug

        //DrawDebugSphere(    
        //                    GetWorld(),
        //                    GetCollisionComponent()->GetComponentLocation(),
        //                    GetCollisionComponent()->GetScaledSphereRadius(),
        //                    8,
        //                    FColor::White,
        //                    false,
        //                    -1.f
        //                );

        //-------------------
    }
}

void UStrategyMovementComponent::ZoomIn()
{
    if (!bCanMoveCamera)    return;

    CameraRadius -= CameraZoomSpeed * FastMoveValue;
    CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);

    //RepositionCamera();
}

void UStrategyMovementComponent::ZoomOut()
{
    if (!bCanMoveCamera)    return;

    CameraRadius += CameraZoomSpeed * FastMoveValue;
    CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);

    //RepositionCamera();
}

void UStrategyMovementComponent::RotateLeft()
{
    if (!bCanMoveCamera)    return;

    CameraZAngle -= CameraRotationSpeed * FastMoveValue;

    //RepositionCamera();
}

void UStrategyMovementComponent::RotateRight()
{
    if (!bCanMoveCamera)    return;

    CameraZAngle += CameraRotationSpeed * FastMoveValue;

    //RepositionCamera();
}

void UStrategyMovementComponent::RotateUp()
{
    if (!bCanMoveCamera)    return;

    CameraHeightAngle += CameraRotationSpeed * FastMoveValue;
    CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);

    //RepositionCamera();
}

void UStrategyMovementComponent::RotateDown()
{
    if (!bCanMoveCamera)    return;

    CameraHeightAngle -= CameraRotationSpeed * FastMoveValue;
    CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);

    //RepositionCamera();
}

//---------------

void UStrategyMovementComponent::RepositionCamera()
{
    AActor* Owner = GetOwner();
    if (Owner == nullptr) return;
    
    FVector NewLocation(0.f, 0.f, 0.f);
    FRotator NewRotation(0.f, 0.f, 0.f);

    float sinCameraZAngle = FMath::Sin(FMath::DegreesToRadians(CameraZAngle));
    float cosCameraZAngle = FMath::Cos(FMath::DegreesToRadians(CameraZAngle));

    float sinCameraHeightAngle = FMath::Sin(FMath::DegreesToRadians(CameraHeightAngle));
    float cosCameraHeightAngle = FMath::Cos(FMath::DegreesToRadians(CameraHeightAngle));

    NewLocation.X = cosCameraZAngle * cosCameraHeightAngle * CameraRadius;
    NewLocation.Y = sinCameraZAngle * cosCameraHeightAngle * CameraRadius;
    NewLocation.Z = sinCameraHeightAngle * CameraRadius;

    // do not allow camera component to go under ground - not enough alone, actor also needed to be limited
    float TerrainSurfaceZ = GetLandTerrainSurfaceAtCoord(Owner->GetActorLocation().X + NewLocation.X, Owner->GetActorLocation().Y + NewLocation.Y);
    if (Owner->GetActorLocation().Z + NewLocation.Z < TerrainSurfaceZ + CameraHeight)
    {
        //FVector NewLocation = CameraComponent->GetComponentLocation();
        NewLocation.Z = TerrainSurfaceZ - Owner->GetActorLocation().Z + CameraHeight;
    }

    // new camera location
    CameraComponent->SetRelativeLocation(NewLocation);

    // new camera rotation
    NewRotation = (FVector(0.0f, 0.0f, 0.0f) - NewLocation).Rotation();
    CameraComponent->SetRelativeRotation(NewRotation);
}

//////////////////////////////////////////////////////////////////


void UStrategyMovementComponent::FastMoveInput(float Direction)
{
    if (!bCanMoveCamera)    return;

    // left or right does not matter, to set double speed in any direction
    FastMoveValue = FMath::Abs(Direction) * 2.0f;

    // used as multiplier so must be 1 if not pressed
    if (FastMoveValue == 0.0f)
    {
        FastMoveValue = 1.0f;
    }
}


void UStrategyMovementComponent::RotateInput(float Direction)
{
    if (!bCanMoveCamera)    return;

    // left or right does not matter
    RotateValue = FMath::Abs(Direction);
}


void UStrategyMovementComponent::MoveCameraForwardInput(float Direction)
{
    if (!bCanMoveCamera) return;

    MoveForwardValue = Direction;
}


void UStrategyMovementComponent::MoveCameraRightInput(float Direction)
{
    if (!bCanMoveCamera)    return;

    MoveRightValue = Direction;
}


void UStrategyMovementComponent::MoveCameraUpInput(float Direction)
{
    if (!bCanMoveCamera)    return;

    MoveUpValue = Direction;
}


void UStrategyMovementComponent::ZoomCameraInInput(float Direction)
{
    if (!bCanMoveCamera) return;

    ZoomInValue = Direction;
}


//------------------------------------------------------------


FVector UStrategyMovementComponent::MoveCameraForward(float Direction)
{
    float MovementValue = Direction * CameraMovementSpeed;
    FVector DeltaMovement = MovementValue * GetIsolatedCameraYaw().Vector();
    //FVector NewLocation = GetActorLocation() + DeltaMovement;
    //SetActorLocation(NewLocation);
    return DeltaMovement;
}


FVector UStrategyMovementComponent::MoveCameraRight(float Direction)
{
    float MovementValue = Direction * CameraMovementSpeed;
    FVector DeltaMovement = MovementValue * (FRotator(0.0f, 90.0f, 0.0f) + GetIsolatedCameraYaw()).Vector();
    //FVector NewLocation = GetActorLocation() + DeltaMovement;
    //SetActorLocation(NewLocation);
    return DeltaMovement;
}


FRotator UStrategyMovementComponent::GetIsolatedCameraYaw()
{
    // FRotator containing Yaw only
    return FRotator(0.0f, CameraComponent->GetComponentTransform().Rotator().Yaw, 0.0f);
}

//---------------

float UStrategyMovementComponent::MoveCameraUp(float Direction)
{
    float MovementValue = Direction * CameraMovementSpeed;
    //FVector DeltaMovement = FVector(0.0f, 0.0f, MovementValue);
    //FVector NewLocation = GetActorLocation() + DeltaMovement;
    //NewLocation.Z = FMath::Clamp(NewLocation.Z, CameraRadiusMin, CameraRadiusMax);
    //SetActorLocation(NewLocation);
    return MovementValue;
}

//---------------

void UStrategyMovementComponent::ZoomCameraIn(float Direction)
{
    float MovementValue = Direction * CameraMovementSpeed;                
    CameraRadius += MovementValue;
    CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);

    //RepositionCamera();
}


void UStrategyMovementComponent::TurnCameraUp(float Direction)
{
    CameraHeightAngle -= Direction * CameraRotationSpeed * 10.0f;        
    CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);

    //RepositionCamera();
}


void UStrategyMovementComponent::TurnCameraRight(float Direction)
{
    CameraZAngle += Direction * CameraRotationSpeed * 10.0f;            

    //RepositionCamera();
}


//////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////


float UStrategyMovementComponent::GetLandTerrainSurfaceAtCoord(float XCoord, float YCoord) const
{
    AActor* Owner = GetOwner();
    if (Owner == nullptr) return 0.f;
    
    FCollisionQueryParams TraceParams(FName(TEXT("LandTerrain")), false, Owner);        // TraceTag (info for debugging), bTraceComplex, AddIgnoredActor
    TraceParams.bFindInitialOverlaps = false;                                        // needed

    FHitResult Hit;

    FVector Start = FVector(XCoord, YCoord, Owner->GetActorLocation().Z + CameraRadius);
    FVector End = FVector(XCoord, YCoord, -500.f);

    // todo: ECC_ channels should be set properly !!!
    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Camera, TraceParams);

    if (bHit)
    {
        return Hit.ImpactPoint.Z;    // for shape trace it differs from Location
    }

    return 0.f;        // water level
}

//////////////////////////////////////////////////////////////////