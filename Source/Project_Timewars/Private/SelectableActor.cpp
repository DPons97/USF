// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectableActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ASelectableActor::ASelectableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = DefaultSceneComponent;

	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActorMesh"));
	ActorMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Set the right selection and pre-selection circle color
	SelectionCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionCircle"));
	SelectionCircle->AttachToComponent(ActorMesh, FAttachmentTransformRules::KeepRelativeTransform);

	PreSelectionCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreSelectionCircle"));
	PreSelectionCircle->AttachToComponent(ActorMesh, FAttachmentTransformRules::KeepRelativeTransform);
	
	TCHAR* SelectionCirclePath;
	TCHAR* PreselectionCirclePath; 
	if (OwningTeam == ETeam::Zombie)
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

	SetActorSelected(false);
}

// Called when the game starts or when spawned
void ASelectableActor::BeginPlay()
{
	Super::BeginPlay();

	// Set current data values to default
	ActorData.Health = ActorData.MaxHealth;
	ActorData.Speed = ActorData.MaxSpeed;
}

// Called every frame
void ASelectableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASelectableActor::SetActorSelected(bool isSelected)
{
	Selected = isSelected;
	SelectionCircle->SetVisibility(isSelected);
	SetActorPreSelected(false);
}

void ASelectableActor::SetActorPreSelected(bool isPreSelected) const
{
	PreSelectionCircle->SetVisibility(isPreSelected);
}

bool ASelectableActor::IsSelected() const
{
	return Selected;
}

TEnumAsByte<ETeam::Type> ASelectableActor::GetOwningTeam() const
{
	return OwningTeam;
}