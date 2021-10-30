// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/SelectablePawn.h"
#include "UnitActor.generated.h"

class USelectableGroup;
class UAnimationAsset;
class UAnimSequence;
class UUnitMovementComponent;
class AStrategyAIController;

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API AUnitActor : public ASelectablePawn
{
	GENERATED_BODY()

public:
	AUnitActor(const FObjectInitializer& ObjectInitializer);

	virtual FVector GetVelocity() const override;

	/* Name of Head component of this unit */
	static FName HeadComponentName;

	/** Returns CapsuleComponent subobject **/
	FORCEINLINE USkeletalMeshComponent* GetHeadComponent() const { return Head; }

	/* Name of Right Weapon component of this unit */
	static FName WeaponRComponentName;

	/** Returns Right Weapon subobject **/
	FORCEINLINE UStaticMeshComponent* GetRWeaponComponent() const { return Weapon_R; }

	/* Name of Left Weapon component of this unit */
	static FName WeaponLComponentName;

	/** Returns Left Weapon subobject **/
	FORCEINLINE UStaticMeshComponent* GetLWeaponComponent() const { return Weapon_L; }
	
	/* Name of Shield component of this unit */
	static FName ShieldComponentName;

	/** Returns Shield subobject **/
	FORCEINLINE UStaticMeshComponent* GetShieldComponent() const { return Shield; }

	/* Name of Back (Backpack/Quiver) component of this unit */
	static FName BackComponentName;

	/** Returns Back subobject **/
	FORCEINLINE UStaticMeshComponent* GetBackComponent() const { return Back; }

protected:
    void BeginPlay() override;
	
private:
	UPROPERTY(Category=Unit, VisibleDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Head;

	UPROPERTY(Category=Unit, VisibleDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* Weapon_R;

	UPROPERTY(Category=Unit, VisibleDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* Weapon_L;

	UPROPERTY(Category=Unit, VisibleDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* Shield;
	
	UPROPERTY(Category=Unit, VisibleDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* Back;

	UPROPERTY()
	UUnitMovementComponent* UnitMovementComponent;
};
