// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
// Sets default values
ATrigger::ATrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create a collider to trigger
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionTrigger"));
	BoxCollider->SetBoxExtent(FVector(32.0f, 32.0f, 32.0f));
	BoxCollider->SetCollisionProfileName("Trigger");
	BoxCollider->SetGenerateOverlapEvents(true);
	SetRootComponent(BoxCollider);

}

// Called when the game starts or when spawned
void ATrigger::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ATrigger::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATrigger::OnOverlapEnd);

	
}


void ATrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		APlayerCharacter* character = Cast<APlayerCharacter>(OtherActor);
		character->TurnSpringarmToSideView();
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("Begin Overlapp"));

	}
}

void ATrigger::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		APlayerCharacter* character = Cast<APlayerCharacter>(OtherActor);
		character->ReturnSpringarmToStartingPos();
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("End Overlapp"));

	}
}

