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
	
	SelectionCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionCircle"));
	SelectionCircle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	PreSelectionCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreSelectionCircle"));
	PreSelectionCircle->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SelectionCircleMesh(TEXT("StaticMesh'/Game/UI/S_SelectionCircle.S_SelectionCircle'"));
	if (!ensure(SelectionCircleMesh.Object != nullptr)) return;
	SelectionCircle->SetStaticMesh(SelectionCircleMesh.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PreSelectionMesh(TEXT("StaticMesh'/Game/UI/S_PreSelectionCircle.S_PreSelectionCircle'"));
	if (!ensure(PreSelectionMesh.Object != nullptr)) return;
	PreSelectionCircle->SetStaticMesh(PreSelectionMesh.Object);	

	SetActorSelected(false);
}

// Called when the game starts or when spawned
void ASelectableActor::BeginPlay()
{
	Super::BeginPlay();

		
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
