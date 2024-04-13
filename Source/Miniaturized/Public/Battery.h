// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Battery.generated.h"

UCLASS()
class MINIATURIZED_API ABattery : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattery();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	/*static mesh to represent the pickup in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BatteryMesh;

	/*returm the mesh for the pickup*/
	FORCEINLINE class UStaticMeshComponent* Getmesh() const { return BatteryMesh; }


};