// Fill out your copyright notice in the Description page of Project Settings.


#include "Selectables/UnitActor.h"


#include "SelectableGroup.h"
#include "UnitAIController.h"
#include "Selectables/UnitMovementComponent.h"

FName AUnitActor::HeadComponentName(TEXT("Head"));
FName AUnitActor::WeaponRComponentName(TEXT("Weapon_R"));
FName AUnitActor::WeaponLComponentName(TEXT("Weapon_L"));
FName AUnitActor::ShieldComponentName(TEXT("Shield"));
FName AUnitActor::BackComponentName(TEXT("Back"));

AUnitActor::AUnitActor(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass(Super::SelectableMovementComponentName, UUnitMovementComponent::StaticClass()))
{
	// todo add AZombieAIController
	AIControllerClass = AUnitAIController::StaticClass();
	ActorData.ActorType = EActorType::Unit;
	SetActorTickEnabled(false);
	SetReplicates(true);
	SetReplicatingMovement(false);
	
	UnitMovementComponent = Cast<UUnitMovementComponent>(GetSelectableMovement());
	CurrentGroup = nullptr;

	// ----------------------------------
	// Actor skeletal mesh and animations
	// ----------------------------------
	USkeletalMeshComponent* thisMesh = GetMesh();	
	Head = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(AUnitActor::HeadComponentName);
	if (Head)
	{
		Head->AttachToComponent(thisMesh, FAttachmentTransformRules::KeepRelativeTransform);
		Head->SetCollisionProfileName(TEXT("NoCollision"));
		Head->SetRelativeLocation(FVector::ZeroVector);
	}
	
	Weapon_R = CreateOptionalDefaultSubobject<UStaticMeshComponent>(AUnitActor::WeaponRComponentName);
	if (Weapon_R)
	{
		Weapon_R->SetupAttachment(thisMesh, TEXT("Weapon_Socket_R"));
		Weapon_R->SetCollisionProfileName(TEXT("NoCollision"));
	}
	
	Weapon_L = CreateOptionalDefaultSubobject<UStaticMeshComponent>(AUnitActor::WeaponLComponentName);
	if (Weapon_L)
	{
		Weapon_L->SetupAttachment(thisMesh,  TEXT("Weapon_Socket_L"));
		Weapon_L->SetCollisionProfileName(TEXT("NoCollision"));
	}
	
	Shield = CreateOptionalDefaultSubobject<UStaticMeshComponent>(AUnitActor::ShieldComponentName);
	if (Shield)
	{
		Shield->SetupAttachment(thisMesh,  TEXT("Shield_Socket"));
		Shield->SetCollisionProfileName(TEXT("NoCollision"));
	}
	
	Back = CreateOptionalDefaultSubobject<UStaticMeshComponent>(AUnitActor::BackComponentName);
	if (Back)
	{
		Back->SetupAttachment(thisMesh,  TEXT("Backpack_Socket"));
		Back->SetCollisionProfileName(TEXT("NoCollision"));
	}
}

FVector AUnitActor::GetVelocity() const
{
	const UPawnMovementComponent* MovementComponent = GetMovementComponent();
	return MovementComponent ? MovementComponent->Velocity * ActorData.Speed : FVector::ZeroVector;
}

void AUnitActor::BeginPlay()
{
	Super::BeginPlay();
}