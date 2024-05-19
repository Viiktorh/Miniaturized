// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MainSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class MINIATURIZED_API UMainSaveGame : public USaveGame
{
	GENERATED_BODY()
	UMainSaveGame();
public:
	UPROPERTY()
	FVector PlayerLocation;
	UPROPERTY()
	FRotator PlayerRotator;
};
