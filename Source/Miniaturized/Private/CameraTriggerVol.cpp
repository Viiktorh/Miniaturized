// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraTriggerVol.h"
#include ""

// Sets default values
ACameraTriggerVol::ACameraTriggerVol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComp->SetupAttachment(RootComponent);

	BlendTime = 1.0f;
}

// Called when the game starts or when spawned
void ACameraTriggerVol::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraTriggerVol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACameraTriggerVol::OnBeginOveralap(AActor* OtherActor)
{
	if (OtherActor->GetClass()->ImplementsInterface(UcameraSwitchInterface::StaticClass()) && OtherActor != this)
	{
		
	}
}

