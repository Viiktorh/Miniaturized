// Fill out your copyright notice in the Description page of Project Settings.


#include "spring.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
// Sets default values
Aspring::Aspring()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void Aspring::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &Aspring::OnOverlapBegin);
}

void Aspring::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor != this)
	{
		APlayerCharacter* character = Cast<APlayerCharacter>(OtherActor);
		character->JumpPad(ForwardMomentum,HowHighMomentum);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("Begin Overlapp"));

	}
}

