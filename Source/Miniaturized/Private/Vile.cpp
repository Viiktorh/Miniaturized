// Fill out your copyright notice in the Description page of Project Settings.


#include "Vile.h"

// Sets default values
AVile::AVile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VialMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VialMesh"));
	RootComponent = VialMesh;
}

// Called when the game starts or when spawned
void AVile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

