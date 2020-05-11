// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectableActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
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

	static ConstructorHelpers::FObjectFinder<UStaticMesh>PlaneMesh(TEXT("StaticMesh'/Game/UI/S_SelectionCircle.S_SelectionCircle'"));
	if (!ensure(PlaneMesh.Object != nullptr)) return;
	
	SelectionCircle->SetStaticMesh(PlaneMesh.Object);

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
}

bool ASelectableActor::IsSelected()
{
	return Selected;
}
