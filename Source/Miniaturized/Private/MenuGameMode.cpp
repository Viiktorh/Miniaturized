// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"
#include "MenuPlayerController.h"
#include "MenuHUD.h"


AMenuGameMode::AMenuGameMode()
{

	PlayerControllerClass = AMenuPlayerController::StaticClass();
	HUDClass = AMenuHUD::StaticClass();
}

