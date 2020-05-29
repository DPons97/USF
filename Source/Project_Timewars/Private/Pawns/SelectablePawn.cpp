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
	
	TCHAR* SelectionCirclePath;
	TCHAR* PreselectionCirclePath; 
	if (ActorData.OwningTeam == ETeam::Zombie)
	{
		SelectionCirclePath = TEXT("StaticMesh'/Game/UI/Selection/S_EnemySlectionCircle.S_EnemySlectionCircle'");
		PreselectionCirclePath = TEXT("StaticMesh'/Game/UI/Selection/S_EnemyPreSelection.S_EnemyPreSelection'");
	} else
	{
		SelectionCirclePath = TEXT("StaticMesh'/Game/UI/Selection/S_SelectionCircle.S_SelectionCircle'");
		PreselectionCirclePath = TEXT("StaticMesh'/Game/UI/Selection/S_PreSelectionCircle.S_PreSelectionCircle'");
	}
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SelectionCircleMesh(SelectionCirclePath);
	if (!ensure(SelectionCircleMesh.Object != nullptr)) return;	
	SelectionCircle->SetStaticMesh(SelectionCircleMesh.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PreSelectionMesh(PreselectionCirclePath);
	if (!ensure(PreSelectionMesh.Object != nullptr)) return;
	PreSelectionCircle->SetStaticMesh(PreSelectionMesh.Object);

    ASelectablePawn::SetActorSelected(false);
}

// Called when the game starts or when spawned
void ASelectablePawn::BeginPlay()
{
	Super::BeginPlay();

	// if (!Controller->Implements<UStrategyCommandInterface>())
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Selectable pawn not implementing strategy command interface! This will lead to not being able to issue commands to this pawn"));
	// }
	// 
	ActorSkeletalMesh = GetMesh();

	ActorData.Health = ActorData.MaxHealth;
	ActorData.Speed = ActorData.MaxSpeed;
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

void ASelectablePawn::ApplyAnimation(TSoftObjectPtr<UAnimationAsset> newAnimation)
{
	if (!newAnimation.IsPending() && ActorSkeletalMesh != nullptr)
	{
		ActorSkeletalMesh->PlayAnimation(newAnimation.Get(), true);
	} else if (newAnimation.IsPending())
	{
		FStreamableManager Streamable;
		const FSoftObjectPath& AnimRef = newAnimation.ToSoftObjectPath();
		newAnimation = Cast<UAnimationAsset>(Streamable.LoadSynchronous(AnimRef));
	}
}
