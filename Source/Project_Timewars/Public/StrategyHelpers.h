// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

/**
 * 
 */
class PROJECT_TIMEWARS_API StrategyHelpers
{
public:
	StrategyHelpers();
	~StrategyHelpers();
	
	static bool GetMousePosition(FVector2D& outMousePosition, APlayerController* PlayerController);
	static bool DeprojectPositionToWorld(FVector2D position, FVector& outWorldPosition, APlayerController* PlayerController, ECollisionChannel CollisionChannel);

	/**
	* Returns the array of actors inside a selection rectangle, with a class filter.
	*    More precise re-implementation of UE4 GetActorsInSelectionRectangle
	* Sample usage:
	*
	*      TArray<AStaticMeshActor*> ActorsInSelectionRect;
	* 		GetActorsInSelectionRectangle<AStaticMeshActor>(FirstPoint,SecondPoint,ActorsInSelectionRect);
	*
	*
	* @param FirstPoint					The first point, or anchor of the marquee box. Where the dragging of the marquee started in screen space.
	* @param SecondPoint					The second point, where the mouse cursor currently is / the other point of the box selection, in screen space.
	* @return OutActors					The actors that are within the selection box according to selection rule
	* @param bIncludeNonCollidingComponents 	Whether to include even non-colliding components of the actor when determining its bounds
	* @param bActorMustBeFullyEnclosed  	The Selection rule: whether the selection box can partially intersect Actor, or must fully enclose the Actor.
	*
	* returns false if selection could not occur. Make sure template class is extending AActor.
	*/
	template <typename ClassFilter>
    static bool GetActorsInSelectionRectangle(const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<ClassFilter*>& OutActors, APlayerController* PC, float CameraZAngle)
	{
		//Is Actor subclass?
		if (!ClassFilter::StaticClass()->IsChildOf(AActor::StaticClass()))
		{
			return false;
		}

		//Run Inner Function, output to Base AActor Array
		TArray<AActor*> OutActorsBaseArray;
		GetActorsInSelectionRectangle(ClassFilter::StaticClass(), FirstPoint, SecondPoint, OutActorsBaseArray, PC, CameraZAngle);

		//Construct casted template type array
		for (AActor* EachActor : OutActorsBaseArray)
		{
			OutActors.Add(CastChecked<ClassFilter>(EachActor));
		}

		return true;
	}
private:
	static void GetActorsInSelectionRectangle(TSubclassOf<class AActor> ClassFilter, const FVector2D& FirstPoint, const FVector2D& SecondPoint, TArray<AActor*>& OutActors, APlayerController* PC, float CameraZAngle);

	static bool IsPointInsidePoly(FVector Point, const FVector Polygon[], uint8 nVertices, APlayerController* PC, FTransform RefTransform);
};
