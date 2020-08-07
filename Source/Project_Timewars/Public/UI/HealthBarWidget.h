// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ObjectMacros.h"

#include "HealthBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UProgressBar* HealthBar;
};
