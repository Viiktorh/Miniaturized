// Fill out your copyright notice in the Description page of Project Settings.

#include "TerrariumKeyBox.h"
#include "Components/BoxComponent.h"

// Sets default values
ATerrariumKeyBox::ATerrariumKeyBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(SceneComponent);
	BoxCollision->SetGenerateOverlapEvents(true);

	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->SetupAttachment(BoxCollision);
	
}

// Called when the game starts or when spawned
void ATerrariumKeyBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATerrariumKeyBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

