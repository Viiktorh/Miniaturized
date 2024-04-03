// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "Components/SplineComponent.h"
#include "Zipline.generated.h"

UCLASS()
class MINIATURIZED_API AZipline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZipline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY()
	APlayerCharacter *Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SplineMovement)
	float Speed = 100;

	float DistanceTravelled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SplineMovement)
	float ZOffset = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spline)
	USplineComponent* SplineComp;

	//Zipline start and end trigger
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spline)
	UBoxComponent* Start;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ColliderSize)
	FVector BoxSize = FVector(32, 32, 32);

	//Moves player along the spline
	UFUNCTION()
	void MoveAlongSpline(const float DeltaTime);

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
};
