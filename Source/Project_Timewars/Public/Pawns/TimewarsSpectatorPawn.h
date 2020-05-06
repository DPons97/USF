// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "TimewarsSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API ATimewarsSpectatorPawn : public ASpectatorPawn
{
	GENERATED_BODY()
public:
	// Constructor
	ATimewarsSpectatorPawn(const FObjectInitializer& ObjectInitializer);

	// Camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* CameraComponent;

	//** Camera XY limit */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    FVector2D CameraXYLimit;

    //** Camera height over terrain */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraHeight;

    //** Camera min height over terrain */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraHeightMin;

    //** Camera max height over terrain */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraHeightMax;

    /** Camera Rotation around Axis Z */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraZAngle;

    /** Camera Height Angle */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraHeightAngle;

    /** Camera Pitch Angle Max */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraHeightAngleMax;

    /** Camera Pitch Angle Min */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraHeightAngleMin;

    /** Camera Radius (Distance) From Pawn Position */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraRadius;

    /** Camera Radius Max */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraRadiusMax;

    /** Camera Radius Min */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraRadiusMin;

    /** Camera Zoom Speed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraZoomSpeed;

    /** Camera Rotation Speed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraRotationSpeed;

    /** Camera Movement Speed */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraMovementSpeed;

    /** Camera Scroll Boundary */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraScrollBoundary;

    /** Should the camera move? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    bool bCanMoveCamera;

    //------------------------------------

	/** Zooms In The Camera */
	void ZoomIn();

	/** Zooms Out The Camera */
	void ZoomOut();

	/** Rotate The Camera Left */
	void RotateLeft();

	/** Rotate The Camera Right */
	void RotateRight();

	/** Rotate The Camera Up */                                
	void RotateUp();

	/** Rotate The Camera Down */                            
	void RotateDown();

	//---

	/** Calculates the new Location and Rotation of The Camera */
	void RepositionCamera();

	//------------------------------------

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

	/** Input recieved to move the camera right
	* @param direction - (1.0 for right, -1.0 for left)
	*/
	void ZoomCameraInInput(float Direction);

	//------------------------------------
	
	/** Tick Function, handles keyboard inputs */
	virtual void Tick(float DeltaSeconds) override;

	//------------------------------------

	// detect landscape and terrain static-mesh
	// usage: RTS Obstacle and RTS Building placement onto landscape, terrain static-mesh
	float GetLandTerrainSurfaceAtCoord(float XCoord, float YCoord) const;

	//------------------------------------
	
protected:
	
	
private:

    /** Sets up player inputs
    *    @param InputComponent - Input Component
    */
    void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

	//------------------------------------

	// set them to +/-1 to get player input from keyboard
	float FastMoveValue;                                            // movement speed multiplier : 1 if shift unpressed, 2 is pressed
	float RotateValue;                                              // turn instead of move camera

	float MoveForwardValue;
	float MoveRightValue;
	float MoveUpValue;
	float ZoomInValue;

	//----------------------------------

	/** Moves the camera forward
	* @param direction - (+ forward, - backward)
	*/
	FVector MoveCameraForward(float Direction);

	/** Moves the camera right
	* @param direction - (+ right, - left)
	*/
	FVector MoveCameraRight(float Direction);

	/** Gets the roatation of the camera with only the yaw value
	* @return - returns a rotator that is (0, yaw, 0) of the Camera
	*/
	FRotator GetIsolatedCameraYaw();

	//---

	/** Moves the camera up/down
	* @param direction - (+ up, - down)
	*/
	float MoveCameraUp(float Direction);

	//---

	/** Zooms the camera in/out
	* @param direction - (+ in, - out)
	*/
	void ZoomCameraIn(float Direction);

	/** Turns the camera up/down
	* @param direction - (+ up, - down)
	*/
	void TurnCameraUp(float Direction);

	/** Turns the camera right/left
	* @param direction - (+ right, - left)
	*/
	void TurnCameraRight(float Direction);

	//------------------------------------

};
