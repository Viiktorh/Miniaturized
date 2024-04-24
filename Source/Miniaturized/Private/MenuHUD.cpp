// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuHUD.h"
#include "Engine/Engine.h"

void AMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	
	//if (IsMenuOpen == true) {
		//Options->OnClicked.AddDynamic(this, &UMenuHUD::OptionsClicked); 
	//}
	
}

/*
void AMenuHUD::SwitchWidgets()
{
	if (GEngine && GEngine->GameViewport) {
		//GEngine->GameViewport->AddViewportWidgetContent(SNew(MyWidgetPtr.ToSharedRef()));

		//GEngine->GameViewport->RemoveViewportWidgetContent(SNew(MyWidgetPtr.ToSharedRef()));
	}
	
}*/

