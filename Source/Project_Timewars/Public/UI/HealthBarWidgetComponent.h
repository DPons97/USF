// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_TIMEWARS_API UHealthBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()


public:
	UHealthBarWidgetComponent();

	void BeginPlay() override;
	
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetHealthBarColor(FColor Color);
	
	float Health;

	float MaxHealth;

private:
	void RotateToFaceCamera();

	APlayerCameraManager *CameraManager;

	float HealthPercentage;

	class UHealthBarWidget * HealthbarWidget;
};


