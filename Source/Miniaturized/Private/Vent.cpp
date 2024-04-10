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
	RootComponent = VentCollision;
	VentCollision->OnComponentBeginOverlap.AddDynamic(this, &AVent::OnBoxBeginOverlap);

	bCanStop = false;
	bCanStart = true;

	Speed = 15.0f;
	//RotationDegree = DeltaTime * Speed;
	//FRotator VentRotation = FRotator(0, RotationDegree, 0);
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

}

/*void AVent::RotateVent()
{
	VentMesh->AddRelativeRotation(VentRotation);
}*/


/*void AVent::StopRotation()
{
	Speed -= ;
	if (Speed <= 0) {
		bCanStart = false;
		Speed = 0;
	}
	VentMesh->AddRelativeRotation(VentRotation);
}*/


void AVent::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bCanStop = true;
	//StopRotation();

}

