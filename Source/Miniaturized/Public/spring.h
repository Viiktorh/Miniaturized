// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "spring.generated.h"

UCLASS()
class MINIATURIZED_API Aspring : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aspring();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = JumpInput)
	double ForwardMomentum = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = JumpInput)
	double HowHighMomentum = 100;

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
};
