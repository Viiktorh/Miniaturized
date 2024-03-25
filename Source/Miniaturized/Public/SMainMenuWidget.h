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

	/*
	// creates a static menu with predefined buttons
	SLATE_UI_COMMAND()
	FMenuBuilder MenuBarBuilder(CommandList);
	{
	MenuBarBuilder.AppPullDownMenu(TEXT("Menu"), TEXT("Open Menu"), FNewMenuDelegate::CreateRaw( &FMenus::FillMenuEntries))
	}

	return MenuBarBuilder.MakeWidget();

	static void FillMenuEntries(FMenuBuilder& MenuBuilder){

		//sound adjustments
		MenuBarBuilder.AddWidget(SNew(SVolumeControl), TEXT("Volume"));

		//Resume play 
		MenuBuilder.AddEditableText  //not sure , but here refer to Toolbar 


	}


	ToolBarBuilder GameToolBarBuilder( InCommandList){
		GameToolBarBuilder.AddToolBarButton(FLevelEditorCommands::Get().Simulate);
		
	}
	return GameToolBarBuilder.MakeWidget();


	*/


};
