// Fill out your copyright notice in the Description page of Project Settings.


#include "SMainMenuWidget.h"

#define LOCTEXT_NAMESPACE "MainManu"


void SMainMenuWidget::Construct(const FArguments& InArgs)
{
	const FMargin ContentPadding = FMargin(500.f, 300.f);
	//localization of the text (able to translate it to other languages!)
	const FText Title = LOCTEXT("GameTitle", "Miniaturized");

	//this is where all the UI is
	ChildSlot
		[
			//lets overlay different things on top of each other, like on canvas in blueprint
			SNew(SOverlay)
			+ SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill)
			[
				//background fill image
				SNew(SImage).ColorAndOpacity(FColor::Black)
			]
			

			+SOverlay::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).Padding(ContentPadding)
			[
				SNew(SVerticalBox)
				+SVerticalBox::Slot()
				[
					SNew(STextBlock).Text(Title)
				]
			]


		];
}
