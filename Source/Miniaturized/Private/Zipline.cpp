// Fill out your copyright notice in the Description page of Project Settings.


#include "Zipline.h"


// Sets default values
AZipline::AZipline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Spline component
	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComp->SetupAttachment(GetRootComponent());

	//Start and end triggers
	Start = CreateDefaultSubobject<UBoxComponent>(TEXT("ZiplineStartTrigger"));
	Start->SetupAttachment(SplineComp);
	Start->SetBoxExtent(BoxSize);

	Start->SetCollisionProfileName("Trigger");
	Start->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void AZipline::BeginPlay()
{
	Super::BeginPlay();
	//Only needed while zipline is in use
	SetActorTickEnabled(false);

	//Sets the trigger at the start of the spline when the game starts
	Start->SetWorldLocation(SplineComp->GetWorldLocationAtSplinePoint(0));

	OnActorBeginOverlap.AddDynamic(this, &AZipline::OnOverlapBegin);
}

// Called every frame
void AZipline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveAlongSpline(DeltaTime);

}

void AZipline::MoveAlongSpline(const float DeltaTime)
{
	if (DistanceTravelled > SplineComp->GetSplineLength())
	{
		SetActorTickEnabled(false);
		DistanceTravelled = 0;
		return;
	}

	DistanceTravelled = (DeltaTime * Speed) + DistanceTravelled;
	FVector ActorPosition = SplineComp->GetLocationAtDistanceAlongSpline(DistanceTravelled, ESplineCoordinateSpace::World);
	ActorPosition.Z += ZOffset;
	Player->SetActorLocation(ActorPosition);
}

void AZipline::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Display, TEXT("Started zipline"));
		Player = Cast<APlayerCharacter>(OtherActor);
		SetActorTickEnabled(true);
	}
}

