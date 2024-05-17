// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Countdown.generated.h"

UCLASS()
class MINIATURIZED_API ACountdown : public AActor
{
	GENERATED_BODY()
	/* the inspiration for my code is: https://github.com/Taheri-meisam/March21CountDown.git */
public:	
	// Sets default values for this actor's properties
	ACountdown();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void CountdownBegin();

	UFUNCTION()
	void CountdownFinished();

	UFUNCTION()
	void ResetCountdown();

	float Time = 60.0f;  // 120.0f

	FTimerHandle CountdownTimer;

	UTextRenderComponent* CountdownText = nullptr;

	UPROPERTY(EditAnywhere)
	USoundBase* CountdownSound{ nullptr };

	
	bool bCollisionEnabled;

	FString FormattedTime;

	UPROPERTY(EditAnywhere)
	UBoxComponent* TriggerBoxTimer;

	/*start timer*/

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//APlayerCharacter* bHasRespawned;
	//APlayerCharacter* CurrentVialsRef;

};
