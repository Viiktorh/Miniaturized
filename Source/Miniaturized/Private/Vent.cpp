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
	

	VentButtonCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("VentButtonCollision"));
	//VentButtonCollision->SetupAttachment(VentMesh);
	
	VentCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("VentCollision"));
	VentCollision->SetupAttachment(VentMesh);
	VentCollision->SetCollisionProfileName("BlockAllDynamic");

	bCanStop = false;
	bCanStart = true;
	bBoxIsPassed = false;

	RotationDegree = 10.0f;
	VentRotation = FRotator(0, 0, 0);
	VentRotation.Pitch = RotationDegree;
	
}

// Called when the game starts or when spawned
void AVent::BeginPlay()
{
	Super::BeginPlay();
	bCanStart = true;
	
	VentButtonCollision->OnComponentBeginOverlap.AddDynamic(this, &AVent::OnBoxBeginOverlap);
	//VentCollision->OnComponentBeginOverlap.AddDynamic(this, &AVent::OnBoxBeginOverlap);

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
	
	VentRotation.Pitch -=0.01f;
	if (VentRotation.Pitch <= 0.0f) {
		bCanStop = true;
		VentRotation.Pitch = 0.0f;
		RotationDegree = 0.0f;
		
	}
	VentMesh->AddWorldRotation(VentRotation);
}


void AVent::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);
	if (Character != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1,2,FColor::Black,TEXT("U touch me"));
		bCanStart = false;
		
		VentButtonCollision->OnComponentBeginOverlap.RemoveAll(this);
		VentButtonCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//VentCollision->OnComponentBeginOverlap.RemoveAll(this);
		

	}
	

}

void AVent::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bBoxIsPassed = true;
	
}

