// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "CameraTriggerVol.generated.h"

UCLASS()
class MINIATURIZED_API ACameraTriggerVol : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraTriggerVol();

protected:
	

public:	
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ACameraActor *CameraToSwitchTo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float BlendTime;

	UPROPERTY(EditAnywhere, Category = "CameraSwitchSettings")
	bool ColliderCover;


	 virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;
	 virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	

};
