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

	Speed = 10.0f;
	RotationDegree = Speed;
	VentRotation = FRotator(0, 0, 0);
	VentRotation.Pitch = RotationDegree;
	
}

// Called when the game starts or when spawned
void AVent::BeginPlay()
{
	Super::BeginPlay();
	bCanStart = true;
	
}

// Called every frame
void AVent::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	
	if (bCanStart) {
		RotateVent();
	}
	else {
		StopRotation();
	}
	
	
}

void AVent::RotateVent()
{
	VentMesh->AddWorldRotation(VentRotation);
}


void AVent::StopRotation()
{
	Speed --;
	if (Speed <= 0) {
		bCanStart = false;
		Speed = 0;
	}
	VentMesh->AddWorldRotation(VentRotation);
}


void AVent::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);
	if (Character != nullptr)
	{
		bCanStop = true;
		bCanStart = false;
		//OnComponentBeginOverlap.RemoveAll(this);
	}
	

}

void AVent::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bBoxIsPassed = true;
	VentCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

