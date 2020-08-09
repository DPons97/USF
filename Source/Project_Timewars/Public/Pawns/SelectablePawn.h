// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StrategySelectionInterface.h"
#include "GameFramework/Pawn.h"
#include "SelectablePawn.generated.h"

class AStrategyAIController;
class UHealthBarWidgetComponent;
class UCapsuleComponent;
class UArrowComponent;
class IStrategyCommandInterface;
class ATimewarsSpectatorPawn;

UCLASS()
class PROJECT_TIMEWARS_API ASelectablePawn :
    public APawn, public IStrategySelectionInterface
{
	GENERATED_BODY()
	
public:	
    // Sets default values for this actor's properties
    ASelectablePawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void SetActorSelected(bool isSelected) override;

	virtual void SetActorPreSelected(bool isPreSelected) override;

	IStrategyCommandInterface* GetControllerInterface();

	UFUNCTION(BlueprintCallable)
	ATimewarsSpectatorPawn* GetOwnerPlayerPawn() const { return OwnerPlayerPawn; }

	UFUNCTION(BlueprintCallable)
    void SetOwnerPlayerPawn(ATimewarsSpectatorPawn* NewOwner);

	UFUNCTION(BlueprintCallable)
	AStrategyAIController* GetStrategyController() const {	return StrategyAIController; }

	UPROPERTY(EditDefaultsOnly)
	FSelectableData ActorData; 

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
	/** Returns Mesh subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Mesh; }

	/** Name of the MeshComponent. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	static FName MeshComponentName;

	/** Returns CharacterMovement subobject **/
	FORCEINLINE UPawnMovementComponent* GetSelectableMovement() const { return SelectableMovementComponent; }

	/** Name of the CharacterMovement component. Use this name if you want to use a different class (with ObjectInitializer.SetDefaultSubobjectClass). */
	static FName SelectableMovementComponentName;

	/** Returns CapsuleComponent subobject **/
	FORCEINLINE UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }

	/** Name of the CapsuleComponent. */
	static FName CapsuleComponentName;

	/** Returns CapsuleComponent subobject **/
	FORCEINLINE UStaticMeshComponent* GetSelectionCircleComponent() const { return SelectionCircleComponent; }

	/** Name of the CapsuleComponent. */
	static FName SelectionCircleName;

	/** Returns CapsuleComponent subobject **/
	FORCEINLINE UStaticMeshComponent* GetPreSelectionCircleComponent() const { return PreSelectionCircleComponent; }

	/** Name of the CapsuleComponent. */
	static FName PreSelectionCircleName;

	/** Returns CapsuleComponent subobject **/
	FORCEINLINE UHealthBarWidgetComponent* GetHealthbarComponent() const { return HealthbarComponent; }

	/** Name of the CapsuleComponent. */
	static FName HealthbarComponentName;

#if WITH_EDITORONLY_DATA
	/** Component shown in the editor only to indicate character facing */
	UPROPERTY()
    UArrowComponent* ArrowComponent;
#endif

private:
	/** The CapsuleComponent being used for movement collision (by CharacterMovement). Always treated as being vertically aligned in simple collision check functions. */
	UPROPERTY(Category=Selectable, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;
	
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category=Selectable, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
    USkeletalMeshComponent* Mesh;

	/** Movement component used for movement logic in various movement modes (walking, falling, etc), containing relevant settings and functions to control movement. */
	UPROPERTY(Category=Selectable, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
    UPawnMovementComponent* SelectableMovementComponent;

	UPROPERTY(Category=Selectable, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
    UHealthBarWidgetComponent* HealthbarComponent;
	
	UPROPERTY(Category=Selectable, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
    UStaticMeshComponent* SelectionCircleComponent;

	UPROPERTY(Category=Selectable, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
    UStaticMeshComponent* PreSelectionCircleComponent;
	
	ATimewarsSpectatorPawn* OwnerPlayerPawn;

	AStrategyAIController* StrategyAIController;

	UPROPERTY(VisibleAnywhere)
    TArray<TSoftObjectPtr<UStaticMesh>> SelectionCircles;

	UPROPERTY(VisibleAnywhere)
	TArray<TSoftObjectPtr<UStaticMesh>> PreSelectionCircles;

	static UStaticMesh* GetLazyLoadedMesh(TSoftObjectPtr<UStaticMesh> BaseMesh);

};