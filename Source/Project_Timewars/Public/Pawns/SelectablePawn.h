// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StrategySelectionInterface.h"
#include "GameFramework/Character.h"
#include "SelectablePawn.generated.h"


UCLASS()
class PROJECT_TIMEWARS_API ASelectablePawn :
    public ACharacter, public IStrategySelectionInterface
{
	GENERATED_BODY()
	
public:	
    // Sets default values for this actor's properties
    ASelectablePawn();

	virtual void SetActorSelected(bool isSelected) override;

	virtual void SetActorPreSelected(bool isPreSelected) override;

	class IStrategyCommandInterface* GetControllerInterface();

	UFUNCTION(BlueprintCallable)
	class ATimewarsSpectatorPawn* GetOwnerPlayerPawn() const { return OwnerPlayerPawn; }

	UFUNCTION(BlueprintCallable)
    void SetOwnerPlayerPawn(ATimewarsSpectatorPawn* NewOwner);

	UPROPERTY(EditDefaultsOnly)
	FSelectableData ActorData; 

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class UHealthBarWidgetComponent* HealthbarComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* SelectionCircleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* PreSelectionCircleComponent;

	// Actor mesh and animations
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    class USkeletalMeshComponent* ActorSkeletalMesh;

private:
	ATimewarsSpectatorPawn* OwnerPlayerPawn;

	UPROPERTY(VisibleAnywhere)
    TArray<TSoftObjectPtr<UStaticMesh>> SelectionCircles;

	UPROPERTY(VisibleAnywhere)
	TArray<TSoftObjectPtr<UStaticMesh>> PreSelectionCircles;

	static class UStaticMesh* GetLazyLoadedMesh(TSoftObjectPtr<UStaticMesh> BaseMesh);

};