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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ACameraActor *CameraToSwitchTo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float BlendTime;


	 virtual void NotifyActorBeginOverlap(AActor *OtherActor);
	 virtual void NotifyActorEndOverlap(AActor *OtherActor);
	 //void (AActor* OtherActor);
	

};
