// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/SelectablePawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/StreamableManager.h"
#include "Project_Timewars/Public/AI/UnitAIController.h"
#include "UObject/ConstructorHelpers.h"

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

	// Set the right selection and pre-selection circle color
	SelectionCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionCircle"));
	SelectionCircle->AttachToComponent(ActorSkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform);
	SelectionCircle->SetCollisionProfileName(TEXT("NoCollision"));

	PreSelectionCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreSelectionCircle"));
	PreSelectionCircle->AttachToComponent(ActorSkeletalMesh, FAttachmentTransformRules::KeepRelativeTransform);
	PreSelectionCircle->SetCollisionProfileName(TEXT("NoCollision"));

	TCHAR* SelectionCirclePath = TEXT("StaticMesh'/Game/UI/Selection/S_SelectionCircle.S_SelectionCircle'");
	TCHAR* PreselectionCirclePath = TEXT("StaticMesh'/Game/UI/Selection/S_PreSelectionCircle.S_PreSelectionCircle'");

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SelectionCircleMesh_Survivors(SelectionCirclePath);
	if (!ensure(SelectionCircleMesh_Survivors.Object != nullptr)) return;	
	SelectionCircle_Survivors = SelectionCircleMesh_Survivors.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PreSelectionMesh_Survivors(PreselectionCirclePath);
	if (!ensure(PreSelectionMesh_Survivors.Object != nullptr)) return;
	PreSelectionCircle_Survivors = PreSelectionMesh_Survivors.Object;

	SelectionCirclePath = TEXT("StaticMesh'/Game/UI/Selection/S_EnemySlectionCircle.S_EnemySlectionCircle'");
	PreselectionCirclePath = TEXT("StaticMesh'/Game/UI/Selection/S_EnemyPreSelection.S_EnemyPreSelection'");

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SelectionCircleMesh_Zombies(SelectionCirclePath);
	if (!ensure(SelectionCircleMesh_Zombies.Object != nullptr)) return;	
	SelectionCircle_Zombies = SelectionCircleMesh_Zombies.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PreSelectionMesh_Zombies(PreselectionCirclePath);
	if (!ensure(PreSelectionMesh_Zombies.Object != nullptr)) return;
	PreSelectionCircle_Zombies = PreSelectionMesh_Zombies.Object;   
	
	ASelectablePawn::SetActorSelected(false);
}

// Called when the game starts or when spawned
void ASelectablePawn::BeginPlay()
{
	Super::BeginPlay();
	ActorSkeletalMesh = GetMesh();

	ActorData.Health = ActorData.MaxHealth;
	ActorData.Speed = ActorData.MaxSpeed;
    
	if (ActorData.OwningTeam == ETeam::Zombie)
	{
		SelectionCircle->SetStaticMesh(SelectionCircle_Zombies);
		PreSelectionCircle->SetStaticMesh(PreSelectionCircle_Zombies);
	} else
	{
		SelectionCircle->SetStaticMesh(SelectionCircle_Survivors);
		PreSelectionCircle->SetStaticMesh(PreSelectionCircle_Survivors);
	}
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