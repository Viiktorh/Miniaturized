// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "cameraSwitchInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UcameraSwitchInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINIATURIZED_API IcameraSwitchInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	 * 
	 * @param Tag 
	 */
	UFUNCTION()
	virtual void RunOnTagOverlap(FString Tag) {};
	UFUNCTION()
	virtual void RunOnTagEndOverlap(FString Tag) {};

};
