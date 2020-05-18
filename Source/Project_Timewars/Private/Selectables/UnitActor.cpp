// Fill out your copyright notice in the Description page of Project Settings.


#include "Selectables/UnitActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/StreamableManager.h"

AUnitActor::AUnitActor()
{
	ActorData.ActorType = EActorType::Unit;
}

void AUnitActor::BeginPlay()
{
	Super::BeginPlay();

	ApplyAnimation(IdleAnimation);
}

void AUnitActor::Move(FVector ClickPosition)
{
	
}

