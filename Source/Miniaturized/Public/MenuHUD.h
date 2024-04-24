// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/Button.h"
#include "MenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class MINIATURIZED_API AMenuHUD : public AHUD
{
	GENERATED_BODY()
	
protected:


	virtual void BeginPlay() override;

public:
	/*UPROPERTY();
	UUserWidget* MenuHUD;

	UPROPERTY()
	UButton* Options;
	
	UPROPERTY()
	UButton* Exit;
	
	UPROPERTY()
	UButton* Resume; 
	
	UPROPERTY()
	UButton* ReturnToLastCheckpoint;
	
	void SwitchWidgets();

	bool ButtonIsPressed=false;

	bool IsMenuOpen = false;
*/
	


};
