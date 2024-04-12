// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerCharacter.h"
#include "Vent.generated.h"

UCLASS()
class MINIATURIZED_API AVent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	bool bCanStop;

	bool bCanStart;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	UStaticMeshComponent* VentMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Collision")
	UBoxComponent* VentButtonCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	UBoxComponent* VentCollision;

	
	void RotateVent();

	bool bBoxIsPassed; //BOOLEAN WITH THE SAME NAME FOR TIMER TO ACTIVATE AFTER RETURNING TO CHECKPOINT

	void StopRotation();

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float Speed;

	int RotationDegree;

	FRotator VentRotation;
};
