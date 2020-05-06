// Fill out your copyright notice in the Description page of Project Settings.


#include "TimewarsSpectatorPawn.h"


#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"


ATimewarsSpectatorPawn::ATimewarsSpectatorPawn(const FObjectInitializer& ObjectInitializer)
{    
    // enable Tick function
    PrimaryActorTick.bCanEverTick = true;

    // disable standard WASD movement
    bAddDefaultMovementBindings = false;

    // not needed Pitch Yaw Roll
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;        
    bUseControllerRotationRoll = false;

    // collision
    GetCollisionComponent()->SetGenerateOverlapEvents(false);
    GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCollisionComponent()->SetCollisionProfileName(TEXT("NoCollision"));
    GetCollisionComponent()->SetSimulatePhysics(false);

    // set defaults
    CameraXYLimit = FVector2D(25000.f, 25000.f);
    CameraHeight = 2500.f;                    
    CameraHeightMin = 300.f;                // 100 for debugging
    CameraHeightMax = 5000.f;

    CameraRadius = 3800.f;                    
    CameraRadiusMin = 1000.f;                // 100 for debugging
    CameraRadiusMax = 8000.f;                

    CameraZAngle = 0.f;                        // yaw

    CameraHeightAngle = 45.f;                // pitch
    CameraHeightAngleMin = 15.f;
    CameraHeightAngleMax = 60.f;

    CameraZoomSpeed = 200.f;                // wheel
    CameraRotationSpeed = 10.f;                // wheel + ctrl
    CameraMovementSpeed = 3000.f;            // in all directions

    CameraScrollBoundary = 25.f;            // screen edge width

    bCanMoveCamera = true;

    // intialize the camera
    CameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("StrategyCameraComponent"));
    CameraComponent->SetupAttachment(RootComponent);
    CameraComponent->bUsePawnControlRotation = false;

    RepositionCamera();
}


void ATimewarsSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (!PlayerInputComponent) return;

    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // action mappings

    // mouse zoom
    PlayerInputComponent->BindAction("ZoomOutByWheel", IE_Pressed, this, &ATimewarsSpectatorPawn::ZoomOut);
    PlayerInputComponent->BindAction("ZoomInByWheel", IE_Pressed, this, &ATimewarsSpectatorPawn::ZoomIn);

    // mouse rotate (+Ctrl or +Alt)
    // unnecessary...
    //PlayerInputComponent->BindAction("RotateLeftByWheel", IE_Pressed, this, &ATimewarsSpectatorPawn::RotateLeftByWheel);
    //PlayerInputComponent->BindAction("RotateRightByWheel", IE_Pressed, this, &ATimewarsSpectatorPawn::RotateRightByWheel);
    // needed...
    PlayerInputComponent->BindAction("RotateUpByWheel", IE_Pressed, this, &ATimewarsSpectatorPawn::RotateUp);
    PlayerInputComponent->BindAction("RotateDownByWheel", IE_Pressed, this, &ATimewarsSpectatorPawn::RotateDown);

    // axis mappings

    // keyboard move (WASD, Home/End)
    PlayerInputComponent->BindAxis("MoveForward", this, &ATimewarsSpectatorPawn::MoveCameraForwardInput);
    PlayerInputComponent->BindAxis("MoveRight", this, &ATimewarsSpectatorPawn::MoveCameraRightInput);
    PlayerInputComponent->BindAxis("MoveUp", this, &ATimewarsSpectatorPawn::MoveCameraUpInput);
    PlayerInputComponent->BindAxis("ZoomIn", this, &ATimewarsSpectatorPawn::ZoomCameraInInput);

    // double speed (WASD +Shift)
    PlayerInputComponent->BindAxis("FastMove", this, &ATimewarsSpectatorPawn::FastMoveInput);
    // yaw and pitch (WASD +Ctrl)
    PlayerInputComponent->BindAxis("Rotate", this, &ATimewarsSpectatorPawn::RotateInput);
}


void ATimewarsSpectatorPawn::ZoomIn()
{
    if (!bCanMoveCamera)    return;

    CameraRadius -= CameraZoomSpeed * FastMoveValue;
    CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);

    //RepositionCamera();
}

void ATimewarsSpectatorPawn::ZoomOut()
{
    if (!bCanMoveCamera)    return;

    CameraRadius += CameraZoomSpeed * FastMoveValue;
    CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);

    //RepositionCamera();
}

void ATimewarsSpectatorPawn::RotateLeft()
{
    if (!bCanMoveCamera)    return;

    CameraZAngle -= CameraRotationSpeed * FastMoveValue;

    //RepositionCamera();
}

void ATimewarsSpectatorPawn::RotateRight()
{
    if (!bCanMoveCamera)    return;

    CameraZAngle += CameraRotationSpeed * FastMoveValue;

    //RepositionCamera();
}

void ATimewarsSpectatorPawn::RotateUp()
{
    if (!bCanMoveCamera)    return;

    CameraHeightAngle += CameraRotationSpeed * FastMoveValue;
    CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);

    //RepositionCamera();
}

void ATimewarsSpectatorPawn::RotateDown()
{
    if (!bCanMoveCamera)    return;

    CameraHeightAngle -= CameraRotationSpeed * FastMoveValue;
    CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);

    //RepositionCamera();
}

//---------------

void ATimewarsSpectatorPawn::RepositionCamera()
{
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
    float TerrainSurfaceZ = GetLandTerrainSurfaceAtCoord(GetActorLocation().X + NewLocation.X, GetActorLocation().Y + NewLocation.Y);
    if (GetActorLocation().Z + NewLocation.Z < TerrainSurfaceZ + CameraHeight)
    {
        //FVector NewLocation = CameraComponent->GetComponentLocation();
        NewLocation.Z = TerrainSurfaceZ - GetActorLocation().Z + CameraHeight;
    }

    // new camera location
    CameraComponent->SetRelativeLocation(NewLocation);

    // new camera rotation
    NewRotation = (FVector(0.0f, 0.0f, 0.0f) - NewLocation).Rotation();
    CameraComponent->SetRelativeRotation(NewRotation);
}

//////////////////////////////////////////////////////////////////


void ATimewarsSpectatorPawn::FastMoveInput(float Direction)
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


void ATimewarsSpectatorPawn::RotateInput(float Direction)
{
    if (!bCanMoveCamera)    return;

    // left or right does not matter
    RotateValue = FMath::Abs(Direction);
}


void ATimewarsSpectatorPawn::MoveCameraForwardInput(float Direction)
{
    if (!bCanMoveCamera) return;

    UE_LOG(LogTemp, Warning, TEXT("Ciao!"))

    MoveForwardValue = Direction;
}


void ATimewarsSpectatorPawn::MoveCameraRightInput(float Direction)
{
    if (!bCanMoveCamera)    return;

    MoveRightValue = Direction;
}


void ATimewarsSpectatorPawn::MoveCameraUpInput(float Direction)
{
    if (!bCanMoveCamera)    return;

    MoveUpValue = Direction;
}


void ATimewarsSpectatorPawn::ZoomCameraInInput(float Direction)
{
    if (!bCanMoveCamera)    return;

    ZoomInValue = Direction;
}


//------------------------------------------------------------


FVector ATimewarsSpectatorPawn::MoveCameraForward(float Direction)
{
    float MovementValue = Direction * CameraMovementSpeed;
    FVector DeltaMovement = MovementValue * GetIsolatedCameraYaw().Vector();
    //FVector NewLocation = GetActorLocation() + DeltaMovement;
    //SetActorLocation(NewLocation);
    return DeltaMovement;
}


FVector ATimewarsSpectatorPawn::MoveCameraRight(float Direction)
{
    float MovementValue = Direction * CameraMovementSpeed;
    FVector DeltaMovement = MovementValue * (FRotator(0.0f, 90.0f, 0.0f) + GetIsolatedCameraYaw()).Vector();
    //FVector NewLocation = GetActorLocation() + DeltaMovement;
    //SetActorLocation(NewLocation);
    return DeltaMovement;
}


FRotator ATimewarsSpectatorPawn::GetIsolatedCameraYaw()
{
    // FRotator containing Yaw only
    return FRotator(0.0f, CameraComponent->GetComponentTransform().Rotator().Yaw, 0.0f);
}

//---------------

float ATimewarsSpectatorPawn::MoveCameraUp(float Direction)
{
    float MovementValue = Direction * CameraMovementSpeed;
    //FVector DeltaMovement = FVector(0.0f, 0.0f, MovementValue);
    //FVector NewLocation = GetActorLocation() + DeltaMovement;
    //NewLocation.Z = FMath::Clamp(NewLocation.Z, CameraRadiusMin, CameraRadiusMax);
    //SetActorLocation(NewLocation);
    return MovementValue;
}

//---------------

void ATimewarsSpectatorPawn::ZoomCameraIn(float Direction)
{
    float MovementValue = Direction * CameraMovementSpeed;                
    CameraRadius += MovementValue;
    CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);

    //RepositionCamera();
}


void ATimewarsSpectatorPawn::TurnCameraUp(float Direction)
{
    CameraHeightAngle -= Direction * CameraRotationSpeed * 10.0f;        
    CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);

    //RepositionCamera();
}


void ATimewarsSpectatorPawn::TurnCameraRight(float Direction)
{
    CameraZAngle += Direction * CameraRotationSpeed * 10.0f;            

    //RepositionCamera();
}


//////////////////////////////////////////////////////////////////


void ATimewarsSpectatorPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

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
            ActualMovement += MoveCameraForward(MoveForwardValue * FastMoveValue * DeltaSeconds);
            ActualMovement += MoveCameraRight(MoveRightValue * FastMoveValue * DeltaSeconds);
        }
        ActualLocation += ActualMovement;

        // vertical movement
        CameraHeight += MoveCameraUp(MoveUpValue * FastMoveValue * DeltaSeconds);
        CameraHeight = FMath::Clamp(CameraHeight, CameraHeightMin, CameraHeightMax);

        // adjust actor height to surface
        float TerrainSurfaceZ = GetLandTerrainSurfaceAtCoord(ActualLocation.X, ActualLocation.Y);
        ActualLocation.Z = TerrainSurfaceZ + CameraHeight;

        // limit movement area
        ActualLocation.X = FMath::Clamp(ActualLocation.X, -CameraXYLimit.X, CameraXYLimit.X);
        ActualLocation.Y = FMath::Clamp(ActualLocation.Y, -CameraXYLimit.Y, CameraXYLimit.Y);

        // move actor
        SetActorLocation(ActualLocation);

        //-------------------
        // tweak camera component relative transform

        // set rotation parameters
        if (RotateValue != 0.f)
        {
            TurnCameraUp(MoveForwardValue * FastMoveValue * DeltaSeconds);
            TurnCameraRight(MoveRightValue * FastMoveValue * DeltaSeconds);
        }

        // set zoom distance
        ZoomCameraIn(ZoomInValue * FastMoveValue * DeltaSeconds);

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


//////////////////////////////////////////////////////////////////


float    ATimewarsSpectatorPawn::GetLandTerrainSurfaceAtCoord(float XCoord, float YCoord) const
{
    FCollisionQueryParams TraceParams(FName(TEXT("LandTerrain")), false, this);        // TraceTag (info for debugging), bTraceComplex, AddIgnoredActor
    TraceParams.bFindInitialOverlaps = false;                                        // needed

    FHitResult Hit;

    FVector Start = FVector(XCoord, YCoord, GetActorLocation().Z + CameraRadius);
    FVector End = FVector(XCoord, YCoord, -500.f);

    // ECC_ channels should be set properly !!!
    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_WorldStatic, TraceParams);

    if (bHit)
    {
        return Hit.ImpactPoint.Z;    // for shape trace it differs from Location
    }

    return 0.f;        // water level
}

//////////////////////////////////////////////////////////////////