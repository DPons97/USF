// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class UStaticMeshComponent* SelectionCircle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class UStaticMeshComponent* PreSelectionCircle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class USceneComponent* DefaultSceneComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool Selected = false;
};
