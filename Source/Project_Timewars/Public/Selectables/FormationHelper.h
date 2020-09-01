// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FormationHelper.generated.h"

/**
 * 	Base representation of a formation:
 *	-> Each 'X' is a unit of the formation
 *	-> Each Space (' ') is 1m of spacing between units
 *	-> Each element of the array is separated by 1m from the previous one
 *	-> 'C' is the Commander of the formation (there can be only ONE commander for each formation)
 *
 *	It is very important that each formation is symmetric with respect to the Commander position
 *	For the sake of simplicity, X spacing can be obtained through an empty string 
 * 	
 * 	In future developments all formations will be loaded from .txt files and will be passed
 * 	directly into this class as strings.
 */
static const TArray<FString> BASE_FORMATION({
	"X X X X X X X X X",
	"",
    "X X X X X X X X X",
	"",
    "X X X X C X X X X",
	"",
    "X X X X X X X X X",
	"",
    "X X X X X X X X X",
});

/**
 * 	Representation of a specific formation after being decoded from its string form
 *
 * 	In future developments this can be also enriched with formation's icon / name / description
 */
USTRUCT(BlueprintType)
struct FFormation
{
	GENERATED_USTRUCT_BODY()

	FString Name;

	FString Description;

	/**
	 * 	Array of spots this formation has available, ordered from the nearest to the furthest to the commander.
	 * 	Coordinates use Commander as origin.
	*/				
	TArray<FVector2D> FormationSpots;

	int GetFormationMaxUnits() const { return FormationSpots.Num(); }
};

// Spacing between different rows of the formation in cm
const float X_SPACING = 100.f;

// Spacing between different columns of the formation in cm
const float Y_SPACING = 100.f;

// Available formations decoded from text file
static TArray<FFormation> AvailableFormations;

static bool bAreFormationsLoaded = false;

// Forward declarations
class ASelectablePawn;

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API UFormationHelper : public UObject
{
	GENERATED_BODY()

public:
	UFormationHelper();
	virtual void BeginDestroy() override;
	
	static int GetFormationMaxUnits(uint8 FormationID);
	
	// Get position of a specific unit inside the group, while using the current formation, with respect to centroid's location
	static void GetLocalUnitsPositions(TArray<ASelectablePawn*> Units, TArray<FVector>& Out_Positions, uint8 FormationID = 0);

	// Get position of a specific unit inside the group, while using the current formation, in world coordinates
	static void GetWorldUnitsPositions(TArray<ASelectablePawn*> Units, FVector Centroid, FVector Orientation, TArray<FVector>& Out_Positions, uint8 FormationID = 0);
	
private:	
	static void DecodeFormations();

	/**
	 * 	Analyzes the given formation at the desired position possibly adding a new available spot to the given TArray of spots 
	 */
	static void CharToFormationSpot(TArray<FString> Formation, TArray<FVector2D>& FormationSpots_ref, int CommanderX, int CommanderY, int offset_x, int offset_y);
};
