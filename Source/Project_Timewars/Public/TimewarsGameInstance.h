// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TimewarsGameInstance.generated.h"


class UFormationHelper;

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API UTimewarsGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UTimewarsGameInstance();

	UFUNCTION(
             BlueprintPure,
             Category = "Units",
             meta = (
                 DisplayName = "Get Formation Helper",
                 Keywords = "FormationHelper"))
	UFormationHelper* FormationHelper();

private:
	UFormationHelper* FormationHelperInstance;
};
