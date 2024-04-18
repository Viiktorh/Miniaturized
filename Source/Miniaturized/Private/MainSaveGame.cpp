// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSaveGame.h"

UMainSaveGame::UMainSaveGame()
{
	PlayerLocation = FVector::Zero();
	PlayerRotator = FRotator::ZeroRotator;
	PlayerCurrentHealth = 0.0f;
	PlayerCurrentAmmo = 0.0f;
}
