// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/SelectablePawn.h"

#include "TimewarsGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StreamableManager.h"
#include "Project_Timewars/Public/AI/UnitAIController.h"
#include "UObject/ConstructorHelpers.h"
#include "TimewarsSpectatorPawn.h"

// Sets default values
ASelectablePawn::ASelectablePawn()
{	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Actor skeletal mesh and animations
	// @todo: make skeletal mesh soft object reference?
	ActorSkeletalMesh = GetMesh();
	ActorSkeletalMesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// ----------------------------------
	// SELECTION CIRCLES
	// ----------------------------------
	SelectionCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionCircle"));
	SelectionCircle->AttachToComponent(ActorSkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform);
	SelectionCircle->SetCollisionProfileName(TEXT("NoCollision"));

	PreSelectionCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreSelectionCircle"));
	PreSelectionCircle->AttachToComponent(ActorSkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform);
	PreSelectionCircle->SetCollisionProfileName(TEXT("NoCollision"));

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
	}

// Called when the game starts or when spawned
void ASelectablePawn::BeginPlay()
{
	Super::BeginPlay();
	ActorSkeletalMesh = GetMesh();

	ActorData.Health = ActorData.MaxHealth;
	ActorData.Speed = ActorData.MaxSpeed;

	SelectionCircle->SetStaticMesh(GetLazyLoadedMesh(SelectionCircles[ActorData.OwningTeam]));
	PreSelectionCircle->SetStaticMesh(GetLazyLoadedMesh(PreSelectionCircles[ActorData.OwningTeam]));

	SetActorSelected(false);
}

void ASelectablePawn::SetActorSelected(bool isSelected)
{
	SelectionCircle->SetVisibility(isSelected);
	SetActorPreSelected(false);
}

void ASelectablePawn::SetActorPreSelected(bool isPreSelected)
{
	PreSelectionCircle->SetVisibility(isPreSelected);
}

IStrategyCommandInterface* ASelectablePawn::GetControllerInterface()
{
	return Cast<IStrategyCommandInterface>(Controller);
}

void ASelectablePawn::SetOwnerPlayerPawn(ATimewarsSpectatorPawn* NewOwner)
{
	this->OwnerPlayerPawn = NewOwner;
	
	SelectionCircle->SetStaticMesh(GetLazyLoadedMesh(SelectionCircles[OwnerPlayerPawn->GetStrategyPlayerState()->TeamColor]));
	PreSelectionCircle->SetStaticMesh(GetLazyLoadedMesh(PreSelectionCircles[OwnerPlayerPawn->GetStrategyPlayerState()->TeamColor]));
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
