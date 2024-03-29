// Fill out your copyright notice in the Description page of Project Settings.


#include "TimewarsGameMode.h"

#include "TimewarsPlayerController.h"
#include "TimewarsSpectatorPawn.h"
#include "StrategyHUD.h"

ATimewarsGameMode::ATimewarsGameMode()
{
    // Defaults
    PlayerControllerClass = ATimewarsPlayerController::StaticClass();
    DefaultPawnClass = ATimewarsSpectatorPawn::StaticClass();
    HUDClass = AStrategyHUD::StaticClass();
}
