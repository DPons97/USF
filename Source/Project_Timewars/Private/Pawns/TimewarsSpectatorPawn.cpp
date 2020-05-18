// Fill out your copyright notice in the Description page of Project Settings.


#include "TimewarsSpectatorPawn.h"
#include "Animation/SkeletalMeshActor.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


ATimewarsSpectatorPawn::ATimewarsSpectatorPawn(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UStrategyMovementComponent>(Super::MovementComponentName))
{
    StrategyMovementComponent = Cast<UStrategyMovementComponent>(MovementComponent);
    
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
    
    // intialize the camera
    CameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("StrategyCameraComponent"));
    CameraComponent->SetupAttachment(RootComponent);
    CameraComponent->bUsePawnControlRotation = false;

    StrategyMovementComponent->SetCameraComponent(CameraComponent);
}


void ATimewarsSpectatorPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);    
}

void ATimewarsSpectatorPawn::BeginPlay()
{
    Super::BeginPlay();
}

void ATimewarsSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (!PlayerInputComponent) return;

    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // action mappings

    // mouse wheel zoom
    PlayerInputComponent->BindAction("ZoomOutByWheel", IE_Pressed, this, &ATimewarsSpectatorPawn::ZoomOut);
    PlayerInputComponent->BindAction("ZoomInByWheel", IE_Pressed, this, &ATimewarsSpectatorPawn::ZoomIn);
    
    // mouse rotate (+Ctrl or +Alt)
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

void ATimewarsSpectatorPawn::StartSelection()
{
    // Block camera from moving
    if (MovementComponent == nullptr) return;
    
    Cast<UStrategyMovementComponent>(StrategyMovementComponent)->bCanMoveCamera = false;
    
}

void ATimewarsSpectatorPawn::EndSelection()
{
    // Block camera from moving
    if (MovementComponent == nullptr) return;
    
    StrategyMovementComponent->bCanMoveCamera = true;
}

void ATimewarsSpectatorPawn::ZoomIn()
{
    if (MovementComponent == nullptr) return;
    
    StrategyMovementComponent->ZoomIn();
}

void ATimewarsSpectatorPawn::ZoomOut()
{
    if (MovementComponent == nullptr) return;
    
    StrategyMovementComponent->ZoomOut();
}

void ATimewarsSpectatorPawn::ZoomCameraInInput(float Direction)
{
    if (MovementComponent == nullptr) return;
    
    StrategyMovementComponent->ZoomCameraInInput(Direction);
}

void ATimewarsSpectatorPawn::FastMoveInput(float Direction)
{
    if (MovementComponent == nullptr) return;
    StrategyMovementComponent->FastMoveInput(Direction);
}

void ATimewarsSpectatorPawn::RotateInput(float Direction)
{
    if (MovementComponent == nullptr) return;
    StrategyMovementComponent->RotateInput(Direction);
}

void ATimewarsSpectatorPawn::MoveCameraForwardInput(float Direction)
{
    if (MovementComponent == nullptr) return;
    StrategyMovementComponent->MoveCameraForwardInput(Direction);
}

void ATimewarsSpectatorPawn::MoveCameraRightInput(float Direction)
{
    if (MovementComponent == nullptr) return;
    StrategyMovementComponent->MoveCameraRightInput(Direction);
}

void ATimewarsSpectatorPawn::MoveCameraUpInput(float Direction)
{
    if (MovementComponent == nullptr) return;
    StrategyMovementComponent->MoveCameraUpInput(Direction);
}

void ATimewarsSpectatorPawn::RotateUp()
{
    if (MovementComponent == nullptr) return;
    StrategyMovementComponent->RotateUp();
}

void ATimewarsSpectatorPawn::RotateDown()
{
    if (MovementComponent == nullptr) return;
    StrategyMovementComponent->RotateDown();
}
