// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class MINIATURIZED_API AMenuHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	/*reference to the menu */
	TSharedPtr<class SMainMenuWidget> MenuWidget;

	/*menu is contained within this container*/
	TSharedPtr<class SWidget> MenuWidgetContainer;

	virtual void BeginPlay() override;
};
