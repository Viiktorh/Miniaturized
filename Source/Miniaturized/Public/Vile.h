// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Vile.generated.h"

UCLASS()
class MINIATURIZED_API AVile : public AActor
{
	GENERATED_BODY()

public:	

	// Sets default values for this actor's properties
	AVile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*static mesh to represent the pickup in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup")
	class UStaticMeshComponent* VialMesh;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
