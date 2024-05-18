// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraTriggerVol.h"
#include "cameraSwitchInterface.h"
#include "PlayerCharacter.h"
#include "Camera/CameraActor.h"


// Sets default values
ACameraTriggerVol::ACameraTriggerVol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetupAttachment(RootComponent);


	CameraToSwitchTo = CreateDefaultSubobject<ACameraActor>(TEXT("LeCapitan"));
	BlendTime = 1.0f;
}


void ACameraTriggerVol::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//Checks if overlapped object is the playercharacter
	if (APlayerCharacter *PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		
		if (APlayerController * PlayerController = Cast<APlayerController>(PlayerCharacter->GetController()))
		{
			PlayerController->SetViewTargetWithBlend(CameraToSwitchTo, BlendTime, EViewTargetBlendFunction::VTBlend_Linear);
			PlayerCharacter->SwitchToTerrariumImc();
			PlayerController->SetControlRotation(CameraToSwitchTo->GetActorRotation());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error in casting playercontroller or changeing view target"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error in casting to playercharacter"))
	}
}

void ACameraTriggerVol::NotifyActorEndOverlap(AActor* OtherActor)
{
	//Only turn back if we're  out of the terrarium area
	if (!ColliderCover){return;}

	//Checks if overlapped object is the playercharacter
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController()))
		{
			PlayerController->SetViewTargetWithBlend(PlayerController->GetPawn(), BlendTime, EViewTargetBlendFunction::VTBlend_Linear);
			PlayerCharacter->SwitchToDefaultImc();
		}
		else
		{
			UE_LOG(LogTemp,Warning,TEXT("Error in casting playercontroller or changeing view target"))
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error in casting to playercharacter"))
	}
}


