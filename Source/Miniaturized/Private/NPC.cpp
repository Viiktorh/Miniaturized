// Fill out your copyright notice in the Description page of Project Settings.
#include "NPC.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "HealingObject.h"
#include "Engine/World.h"


// Sets default values
ANPC::ANPC()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxHealth = 200.0f;
	Health = MaxHealth;

	EnemyIsDead = false;
	bCanDrop = false;
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanDrop)
	{
		int PickUpDrop = 0;
		if (PickUpDrop < 1)
		{
			FVector Location = GetActorLocation();
			UWorld* World = GetWorld();
			if (World != nullptr)
			{
				AActor* SpawnedHealingObject = World->SpawnActor<AActor>(HealthPickUp, Location, FRotator::ZeroRotator, SpawnInfo);
				if (SpawnedHealingObject)
				{
					PickUpDrop++;
				}
			}
		}
	}
}

float ANPC::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,AActor* DamageCauser)
{
	Health -= DamageAmount;
	if (Health <= 0.0f)
	{
		Health = 0.0f;
		Die();
	}
	return DamageAmount;
}

void ANPC::Die()
{
	EnemyIsDead = true;
}







