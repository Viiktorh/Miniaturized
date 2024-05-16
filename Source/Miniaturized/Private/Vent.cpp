// Fill out your copyright notice in the Description page of Project Settings.

#include "Vent.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AVent::AVent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* VentRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scenecomponent"));
	VentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VentMesh"));
	VentMesh->SetupAttachment(VentRoot);
	VentMesh->SetMobility(EComponentMobility::Movable);

	VentButtonCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("VentButtonCollision"));
	VentButtonCollision->SetupAttachment(VentRoot);
	
	VentCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("VentCollision"));
	VentCollision->SetupAttachment(VentRoot);
	VentCollision->SetCollisionProfileName("BlockAllDynamic");
	
	DeathCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DeathCollision"));
	DeathCollision->SetupAttachment(VentRoot);
	

	bCanStop = false;
	bCanStart = true;
	bBoxIsPassed = false;

	RotationDegree = 10.0f;
	VentRotation = FRotator(0, 0, 0);
	VentRotation.Pitch = RotationDegree;
	SetRootComponent(VentRoot);
}

// Called when the game starts or when spawned
void AVent::BeginPlay()
{
	Super::BeginPlay();


	
	VentButtonCollision->OnComponentBeginOverlap.AddDynamic(this, &AVent::OnBoxBeginOverlap);
	//VentButtonCollision->OnComponentEndOverlap.AddDynamic(this, &AVent::OnBoxEndOverlap);
	DeathCollision->OnComponentBeginOverlap.AddDynamic(this, &AVent::DieOnBoxBeginOverlap);

	

}

// Called every frame
void AVent::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	
	if (bBoxIsPassed == false) {
		bCanStart = true;
	}


	if (bCanStart) {
		RotateVent();
	}
	else {
		StopRotation();
	}
	
	
}

void AVent::RotateVent()
{
	VentMesh->AddLocalRotation(VentRotation);
}


void AVent::StopRotation()
{
	
	VentRotation.Pitch -=0.1f;
	if (VentRotation.Pitch <= 0.0f) {
		bCanStop = true;
		VentRotation.Pitch = 0.0f;
		RotationDegree = 0.0f;
		
	}
	VentMesh->AddLocalRotation(VentRotation);
}


void AVent::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATerrariumKeyBox* Actor = Cast<ATerrariumKeyBox>(OtherActor);
	if (Actor != nullptr)
	{

		bCanStart = false;
		
		VentButtonCollision->OnComponentBeginOverlap.RemoveAll(this);
		VentButtonCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		VentCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bBoxIsPassed = true;
	}
}

	
void AVent::DieOnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);
	if (Character != nullptr)
	{
		UGameplayStatics::ApplyDamage(Character, 100.f, Character->GetController(), Character, DamageType); //applies damage equal to character's health to kill them
		DeathCollision->OnComponentBeginOverlap.RemoveAll(this);
	}
}





