// Fill out your copyright notice in the Description page of Project Settings.

#include "TimewarsPlayerController.h"


#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

ATimewarsPlayerController::ATimewarsPlayerController()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;    
}