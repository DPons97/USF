// Fill out your copyright notice in the Description page of Project Settings.


#include "Selectables/UnitActor.h"
#include "UnitAIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/StreamableManager.h"

AUnitActor::AUnitActor()
{
	AIControllerClass = AUnitAIController::StaticClass();
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

UAnimationAsset* AUnitActor::GetAnimation(TSoftObjectPtr<UAnimationAsset> Animation)
{
	if (!Animation.IsPending())
	{
        return Animation.Get();
	} else
	{
		FStreamableManager Streamable;
		const FSoftObjectPath& AnimRef = Animation.ToSoftObjectPath();
		return Cast<UAnimationAsset>(Streamable.LoadSynchronous(AnimRef));
	}
}