// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StrategyUtils.h"

#include "CoreMinimal.h"
#include "StrategyMovementComponent.h"
#include "GameFramework/SpectatorPawn.h"
#include "TimewarsSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API ATimewarsSpectatorPawn : public ASpectatorPawn
{
	GENERATED_UCLASS_BODY()
public:
	void StartSelection();

	void EndSelection();
	
	// Camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* CameraComponent;

	//------------------------------------
	
	/** Tick Function, handles keyboard inputs */
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	void BeginPlay() override;
	
private:
	UStrategyMovementComponent* StrategyMovementComponent;
	
    /** Sets up player inputs
    *    @param InputComponent - Input Component
    */
    void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

	/** Left or Right Shift is pressed
	* @param direction - (1.0 for Right, -1.0 for Left)
	*/
	void FastMoveInput(float Direction);

	/** Left or Right Ctrl is pressed
	* @param direction - (1.0 for Right, -1.0 for Left)
	*/
	void RotateInput(float Direction);  
	
	/** Input recieved to move the camera forward
	* @param direction - (1.0 for forward, -1.0 for backward)
	*/
	void MoveCameraForwardInput(float Direction);

	/** Input recieved to move the camera right
	* @param direction - (1.0 for right, -1.0 for left)
	*/
	void MoveCameraRightInput(float Direction);

	/** Input recieved to move the camera right
	* @param direction - (1.0 for right, -1.0 for left)
	*/
	void MoveCameraUpInput(float Direction);
	
	/** Input recieved to zoom camera
	* @param direction - (1.0 for backwards, -1.0 for forwards)
	*/
	void ZoomCameraInInput(float Direction);

	/** Rotate The Camera Up */                                
	void RotateUp();

	/** Rotate The Camera Down */                            
	void RotateDown();

	/** Zooms In The Camera */
	void ZoomIn();

	/** Zooms Out The Camera */
	void ZoomOut();
};
