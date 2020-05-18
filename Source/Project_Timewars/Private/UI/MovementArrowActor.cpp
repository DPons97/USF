// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MovementArrowActor.h"



#include "StrategyHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AMovementArrowActor::AMovementArrowActor()
{
	SetActorEnableCollision(false);
	
	static ConstructorHelpers::FObjectFinder<UAnimSequence>ArrowAnimSeq(TEXT("AnimSequence'/Game/Resources/Selection/MovementArrow/MovementArrow_Anim.MovementArrow_Anim'"));
	if (!ensure(ArrowAnimSeq.Object != nullptr)) return;	
	MovementArrowAnimSequence = ArrowAnimSeq.Object;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>ArrowSkeletalMesh(TEXT("SkeletalMesh'/Game/Resources/Selection/MovementArrow/MovementArrow.MovementArrow'"));
	if (!ensure(ArrowSkeletalMesh.Object != nullptr)) return;
	GetSkeletalMeshComponent()->SetSkeletalMesh(ArrowSkeletalMesh.Object);
}

void AMovementArrowActor::PlayAnimationAtMousePosition()
{
	if (MovementArrowAnimSequence == nullptr) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0); 
    
	FVector2D MousePosition;
	StrategyHelpers::GetMousePosition(MousePosition, PC);

	FVector MouseToWorld;
	StrategyHelpers::DeprojectPositionToWorld(MousePosition, MouseToWorld, PC, ECollisionChannel::ECC_WorldStatic);

	SetActorLocation(MouseToWorld);
	GetSkeletalMeshComponent()->PlayAnimation(MovementArrowAnimSequence, false);
}


