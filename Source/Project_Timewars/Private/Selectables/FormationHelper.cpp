// Fill out your copyright notice in the Description page of Project Settings.


#include "FormationHelper.h"

#include "UnitActor.h"

UFormationHelper::UFormationHelper()
{
	DecodeFormations();
}

void UFormationHelper::BeginDestroy()
{
	if (AvailableFormations.Num() > 0) UE_LOG(LogTemp, Warning, TEXT("Available formations still existing!"))
	
	Super::BeginDestroy();
}

int UFormationHelper::GetFormationMaxUnits(uint8 FormationID)
{
	return AvailableFormations[FormationID].GetFormationMaxUnits();
}

void UFormationHelper::GetLocalUnitsPositions(TArray<AUnitActor*> Units, TArray<FVector>& Out_Positions,
	uint8 FormationID)
{
	TArray<FVector2D> AvailablePositions = AvailableFormations[FormationID].FormationSpots;
	TArray<AUnitActor*> AvailableUnits = Units;
	
	Out_Positions.SetNum(Units.Num());

	for (int n = 0; n < Units.Num(); n++)
	{
		FVector2D CurrentSpot = AvailablePositions[0];

		int NearestUnitIdx = 0;
		float MinDist =  -1.f;

		// Select unit to fill the new slot
		for (int i = 0; i < AvailableUnits.Num(); i++)
		{			
			if (AvailableUnits[i] != nullptr)
			{
				if (MinDist < 0)
				{
					MinDist = FVector2D::Distance(CurrentSpot, FVector2D(AvailableUnits[i]->GetActorLocation()));
					NearestUnitIdx = i;
				}
				{
					float CurrDist =  FVector2D::Distance(CurrentSpot, FVector2D(AvailableUnits[i]->GetActorLocation()));

					if (CurrDist < MinDist)
					{
						MinDist = CurrDist;
						NearestUnitIdx = i;
					}
				}
			}
		}

		Out_Positions[NearestUnitIdx] = FVector(CurrentSpot.X, CurrentSpot.Y, 0);
		AvailableUnits[NearestUnitIdx] = nullptr;
		AvailablePositions.RemoveAt(0);
	}
}

void UFormationHelper::GetWorldUnitsPositions(TArray<AUnitActor*> Units, FVector Centroid, FVector Orientation,
	TArray<FVector>& Out_Positions,	uint8 FormationID)
{
	GetLocalUnitsPositions(Units, OUT Out_Positions, FormationID);

	for (int i=0; i < Out_Positions.Num(); i++)
	{
		// Orientation
		Out_Positions[i] = Out_Positions[i].RotateAngleAxis(Orientation.Rotation().Yaw, FVector::UpVector);
	
		// Translation
		Out_Positions[i] = Out_Positions[i] + Centroid;	
	}
}

void UFormationHelper::DecodeFormations()
{
	if (AvailableFormations.Num() > 0) return;
	
	// todo load from txt and loop through each available formation
	// todo check validity of formation (see code below)
	TArray<FString> CurrFormation = BASE_FORMATION;

	// Find Commander coordinates
	int CommanderY = 0;
	int CommanderX = 0;
	while (CommanderX < CurrFormation.Num() && !CurrFormation[CommanderX].FindChar('C', CommanderY))
	{
		CommanderX++;
	}

	if (CommanderX == CurrFormation.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("Trying to decode a formation with no commander"))
		return;
	}

	TArray<FVector2D> CurrFormationSpots;
	CurrFormationSpots.Add(FVector2D(0,0));
	const int CurrFormationWidth = CurrFormation[0].Len();
	
	// For each value of offset (+- row ; +- col)	
	for (int i = 0; i <= CurrFormation.Num() / 2; i++)
	{
		if (!CurrFormation[CommanderX + i].IsEmpty())
		{
			// Analyze row with positive offset
			for (int j = 0; j <= CurrFormationWidth / 2; j++)
			{
				CharToFormationSpot(CurrFormation, CurrFormationSpots, CommanderX, CommanderY, i, j);

				if (j > 0)
				{
					CharToFormationSpot(CurrFormation, CurrFormationSpots, CommanderX, CommanderY, i, -j);
				}
			}
		}
		
		if (i > 0)
		{
			if (!CurrFormation[CommanderX - i].IsEmpty())
			{
				// Analyze row with negative offset
				for (int j = 0; j <= CurrFormationWidth / 2; j++)
				{
					CharToFormationSpot(CurrFormation, CurrFormationSpots, CommanderX, CommanderY, -i, j);

					if (j > 0)
					{
						CharToFormationSpot(CurrFormation, CurrFormationSpots, CommanderX, CommanderY, -i, -j);
					}
				}
			} 
		} 
	}

	// todo get Name and description from txt
	FFormation NewFormation;
	NewFormation.Name = "BASE FORMATION";
	NewFormation.Description = "This is a very cool formation :D";
	NewFormation.FormationSpots = CurrFormationSpots;

	AvailableFormations.Add(NewFormation);

	bAreFormationsLoaded = true;
} 


void UFormationHelper::CharToFormationSpot(TArray<FString> Formation, TArray<FVector2D>& FormationSpots_ref, int CommanderX, int CommanderY, int offset_x, int offset_y)
{
	if (Formation[CommanderX + offset_x][CommanderY + offset_y] == 'X')
	{
		FormationSpots_ref.Add(FVector2D(
            offset_x * X_SPACING,
            offset_y * Y_SPACING
        ));
	}
}