// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StrategyUtils.h"

#include "CoreMinimal.h"
#include "TimewarsPlayerController.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"

#include "SelectableActor.generated.h"

UCLASS()
class PROJECT_TIMEWARS_API ASelectableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectableActor();

	void SetActorSelected(bool isSelected);

	void SetActorPreSelected(bool isPreSelected) const;

	bool IsSelected() const;

	TEnumAsByte<ETeam::Type> GetOwningTeam() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class USceneComponent* DefaultSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class UStaticMeshComponent* ActorMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class UStaticMeshComponent* SelectionCircle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* PreSelectionCircle;

	UPROPERTY(EditDefaultsOnly)
	FSelectableData ActorData;

	TEnumAsByte<ETeam::Type> OwningTeam = ETeam::UNDEFINED;
private:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool Selected = false;
};