// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/SelectablePawn.h"

#include "TimewarsGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UI/HealthBarWidgetComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StreamableManager.h"
#include "Project_Timewars/Public/AI/UnitAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "TimewarsSpectatorPawn.h"
#include "StrategyHelpers.h"
#include "Components/ArrowComponent.h"
#include "Engine/CollisionProfile.h"

FName ASelectablePawn::MeshComponentName(TEXT("CharacterMesh0"));
FName ASelectablePawn::SelectableMovementComponentName(TEXT("CharMoveComp"));
FName ASelectablePawn::CapsuleComponentName(TEXT("CollisionCylinder"));
FName ASelectablePawn::SelectionCircleName(TEXT("SelectionCircle"));
FName ASelectablePawn::PreSelectionCircleName(TEXT("PreSelectionCircle"));
FName ASelectablePawn::HealthbarComponentName(TEXT("HealthBar"));

// Sets default values
ASelectablePawn::ASelectablePawn(const FObjectInitializer& ObjectInitializer)
{	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create capsule for collisions
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(ASelectablePawn::CapsuleComponentName);
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	RootComponent = CapsuleComponent;

#if WITH_EDITORONLY_DATA
	ArrowComponent = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (ArrowComponent)
	{
		ArrowComponent->ArrowColor = FColor(150, 200, 255);
		ArrowComponent->bTreatAsASprite = true;
		// ArrowComponent->SpriteInfo.Category = ConstructorStatics.ID_Characters;
		// ArrowComponent->SpriteInfo.DisplayName = ConstructorStatics.NAME_Characters;
		ArrowComponent->SetupAttachment(CapsuleComponent);
		ArrowComponent->bIsScreenSizeScaled = true;
	}
#endif // WITH_EDITORONLY_DATA

	SelectableMovementComponent = CreateDefaultSubobject<UPawnMovementComponent>(ASelectablePawn::SelectableMovementComponentName);
	if (SelectableMovementComponent)
	{
		SelectableMovementComponent->UpdatedComponent = CapsuleComponent;
	}
	
	// Actor skeletal mesh and animations
	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(ASelectablePawn::MeshComponentName);
	if (Mesh)
	{
		Mesh->AlwaysLoadOnClient = true;
		Mesh->AlwaysLoadOnServer = true;
		Mesh->bOwnerNoSee = false;
		Mesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
		Mesh->bCastDynamicShadow = true;
		Mesh->bAffectDynamicIndirectLighting = true;
		Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Mesh->SetupAttachment(CapsuleComponent);
		static FName MeshCollisionProfileName(TEXT("BlockAllDynamic"));
		Mesh->SetCollisionProfileName(MeshCollisionProfileName);
		Mesh->SetGenerateOverlapEvents(false);
		Mesh->SetCanEverAffectNavigation(false);
	}

	// ----------------------------------
	// Selection circles
	// ----------------------------------
	SelectionCircleComponent = CreateOptionalDefaultSubobject<UStaticMeshComponent>(ASelectablePawn::SelectionCircleName);
	if (SelectionCircleComponent)
	{
		SelectionCircleComponent->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
		SelectionCircleComponent->SetCollisionProfileName(TEXT("NoCollision"));
		SelectionCircleComponent->SetCastShadow(false);
		SelectionCircleComponent->SetRelativeLocation(FVector(0,0,10));
	}

	PreSelectionCircleComponent = CreateOptionalDefaultSubobject<UStaticMeshComponent>(ASelectablePawn::PreSelectionCircleName);
	if (PreSelectionCircleComponent)
	{
		PreSelectionCircleComponent->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
		PreSelectionCircleComponent->SetCollisionProfileName(TEXT("NoCollision"));
		PreSelectionCircleComponent->SetCastShadow(false);
		PreSelectionCircleComponent->SetRelativeLocation(FVector(0,0,10));
	}
	
	for (int i=0 ; i < 6; i++)
	{
		FString SelPathStr = "StaticMesh'/Game/UI/Selection/S_SelectionCircle_" + FString::FromInt(i) + ".S_SelectionCircle_" + FString::FromInt(i) + "'";
		FString PrePathStr = "StaticMesh'/Game/UI/Selection/S_PreSelectionCircle_" + FString::FromInt(i) + ".S_PreSelectionCircle_" + FString::FromInt(i) + "'";
		const TCHAR* SelPath = *SelPathStr;
		const TCHAR* PrePath = *PrePathStr;
		
		ConstructorHelpers::FObjectFinder<UStaticMesh>SelectionCircleMesh(SelPath);
		ConstructorHelpers::FObjectFinder<UStaticMesh>PreSelectionCircleMesh(PrePath);
		if (ensure(SelectionCircleMesh.Object != nullptr))
		{
			SelectionCircles.Add(SelectionCircleMesh.Object);
			PreSelectionCircles.Add(PreSelectionCircleMesh.Object);
		}
	}

	// ----------------------------------
	// Health bar
	// ----------------------------------
	HealthbarComponent = CreateOptionalDefaultSubobject<UHealthBarWidgetComponent>(ASelectablePawn::HealthbarComponentName);
	if (HealthbarComponent)
	{
		HealthbarComponent->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
		HealthbarComponent->SetCollisionProfileName(TEXT("NoCollision"));
	}
}

// Called when the game starts or when spawned
void ASelectablePawn::BeginPlay()
{
	Super::BeginPlay();

	ActorData.Health = ActorData.MaxHealth;
	ActorData.Speed = ActorData.MaxSpeed;

	if (!ensure(SelectionCircleComponent != nullptr || PreSelectionCircleComponent != nullptr)) return;
	SelectionCircleComponent->SetStaticMesh(GetLazyLoadedMesh(SelectionCircles[ActorData.OwningTeam]));
	PreSelectionCircleComponent->SetStaticMesh(GetLazyLoadedMesh(PreSelectionCircles[ActorData.OwningTeam]));

	if (!ensure(HealthbarComponent != nullptr)) return;
	HealthbarComponent->Health = ActorData.Health;
	HealthbarComponent->MaxHealth = ActorData.MaxHealth;
	HealthbarComponent->SetHealthBarColor(StrategyHelpers::GetTeamColor(ActorData.OwningTeam));

	SetActorSelected(false);
}

void ASelectablePawn::SetActorSelected(bool isSelected)
{
	SelectionCircleComponent->SetVisibility(isSelected);

	if (ActorData.Health == ActorData.MaxHealth)
	{
		HealthbarComponent->SetVisibility(isSelected);
	}

	SetActorPreSelected(false);
}

void ASelectablePawn::SetActorPreSelected(bool isPreSelected)
{
	PreSelectionCircleComponent->SetVisibility(isPreSelected);
}

IStrategyCommandInterface* ASelectablePawn::GetControllerInterface()
{
	return Cast<IStrategyCommandInterface>(Controller);
}

void ASelectablePawn::SetOwnerPlayerPawn(ATimewarsSpectatorPawn* NewOwner)
{
	this->OwnerPlayerPawn = NewOwner;
	
	SelectionCircleComponent->SetStaticMesh(GetLazyLoadedMesh(SelectionCircles[OwnerPlayerPawn->GetStrategyPlayerState()->TeamColor]));
	PreSelectionCircleComponent->SetStaticMesh(GetLazyLoadedMesh(PreSelectionCircles[OwnerPlayerPawn->GetStrategyPlayerState()->TeamColor]));
	HealthbarComponent->SetHealthBarColor(StrategyHelpers::GetTeamColor(OwnerPlayerPawn->GetStrategyPlayerState()->TeamColor));
}

UStaticMesh* ASelectablePawn::GetLazyLoadedMesh(TSoftObjectPtr<UStaticMesh> BaseMesh)
{
    if (BaseMesh.IsPending())
    {
        const FSoftObjectPath& AssetRef = BaseMesh.ToSoftObjectPath();
        BaseMesh = BaseMesh.LoadSynchronous();
    }
    return BaseMesh.Get();
}
