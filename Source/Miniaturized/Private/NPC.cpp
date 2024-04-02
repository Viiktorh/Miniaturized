// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	health = 1.0f;
	DieDelay = 2.0f;
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPC::TakeDamage(float damage)
{
	health -= damage;
	if (health <= 0.0f) {
		Die();
	}
	GetWorld()->GetTimerManager().SetTimer(DieTimer, this, &ANPC::Die, DieDelay, false);
}

void ANPC::Die()
{
	Destroy();
	

}

