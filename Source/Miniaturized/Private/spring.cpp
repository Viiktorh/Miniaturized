// Fill out your copyright notice in the Description page of Project Settings.

#include "spring.h"
#include "PlayerCharacter.h"
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
	APlayerCharacter* character = Cast<APlayerCharacter>(OtherActor);
	if (character != nullptr && OtherActor != this)
	{
		character->JumpPad(ForwardMomentum,HowHighMomentum);
	}
}

