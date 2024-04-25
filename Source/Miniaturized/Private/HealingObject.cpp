// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingObject.h"

// Sets default values
AHealingObject::AHealingObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BatteryMesh"));
	RootComponent = HealingMesh;
}

// Called when the game starts or when spawned
void AHealingObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealingObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

