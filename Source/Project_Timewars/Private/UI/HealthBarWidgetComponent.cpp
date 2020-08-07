// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HealthBarWidgetComponent.h"

#include "UI/HealthBarWidget.h"
#include "Components/ProgressBar.h"
#include "UObject/ConstructorHelpers.h"


UHealthBarWidgetComponent::UHealthBarWidgetComponent()
{
	ConstructorHelpers::FClassFinder<UHealthBarWidget>HealthbarWidgetClass(TEXT("/Game/UI/HealthBar/WBP_HealthBar"));
	if (!ensure(HealthbarWidgetClass.Class != nullptr)) return;
	WidgetClass = HealthbarWidgetClass.Class;
}

void UHealthBarWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld * World = GetWorld();
	if (World == nullptr) return;
	CameraManager = World->GetFirstPlayerController()->PlayerCameraManager;

	if (Widget == nullptr) return;
	HealthbarWidget = Cast<UHealthBarWidget>(Widget);
	if (HealthbarWidget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Healthbar widget is of wrong class!"))
		return;
	}
}

void UHealthBarWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Compute health fraction
	if (HealthbarWidget == nullptr) return;
	HealthbarWidget->HealthBar->SetPercent(Health / MaxHealth);
	
	RotateToFaceCamera();
}

void UHealthBarWidgetComponent::SetHealthBarColor(FColor Color)
{
	if (HealthbarWidget == nullptr) return;
	HealthbarWidget->HealthBar->SetFillColorAndOpacity(Color);
}

void UHealthBarWidgetComponent::RotateToFaceCamera()
{
	if (Widget == nullptr || CameraManager == nullptr) return;

	const FVector CameraLoc = CameraManager->GetCameraLocation();
	const FVector FacingVector = CameraLoc - GetComponentLocation();

	SetWorldRotation(FacingVector.Rotation());	
}
