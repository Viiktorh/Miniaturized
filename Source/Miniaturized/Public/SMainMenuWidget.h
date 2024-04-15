// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "MenuHUD.h"

/**
 * 
 */
class  SMainMenuWidget: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)

	SLATE_END_ARGS()

	

	/*widget needs construct */
	void Construct(const FArguments& InArgs);

	/*the HUD that created this widget*/
	TWeakObjectPtr<class AMenuHUD> OwningHUD;

	virtual bool SupportsKeyboardFocus() const override { return true; };

	
	// creates a static menu with predefined buttons
	


	


};
