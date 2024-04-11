// Fill out your copyright notice in the Description page of Project Settings.


#include "Vent.h"

// Sets default values
AVent::AVent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	VentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VentMesh"));
	RootComponent = VentMesh;
	VentMesh->SetMobility(EComponentMobility::Movable);

	VentCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("VentCollision"));
	VentCollision->OnComponentBeginOverlap.AddDynamic(this, &AVent::OnBoxBeginOverlap);

	bCanStop = false;
	bCanStart = true;
	bBoxIsPassed = false;

	Speed = 15.0f;
	VentRotation = FRotator(0, RotationDegree, 0);
}

// Called when the game starts or when spawned
void AVent::BeginPlay()
{
	Super::BeginPlay();
	//RotateVent();
}

// Called every frame
void AVent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotationDegree = DeltaTime * Speed;
}

void AVent::RotateVent(float DeltaTime)
{
	VentMesh->AddRelativeRotation(VentRotation);
}


void AVent::StopRotation(float DeltaTime)
{
	Speed --;
	if (Speed <= 0) {
		bCanStart = false;
		Speed = 0;
	}
	VentMesh->AddRelativeRotation(VentRotation);
}


void AVent::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bBoxIsPassed = true;
	bCanStop = true;
	VentCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//StopRotation();

}

