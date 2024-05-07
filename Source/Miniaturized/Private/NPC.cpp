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
	//NPCMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NPCMesh"));

	EnemyIsDead = false;
	DieDelay = 2.0f;
	bCanDrop = false;

	/*
	FVector EnemyLocation(0.0f, 0.0f, 0.0f); //= NPCLocation;
	FRotator EnemyRotation(0.0f, 0.0f, 0.0f);
	*/
	
	
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

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


float ANPC::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,AActor* DamageCauser)
{
	Health -= DamageAmount;
	if (Health <= 0.0f)
	{
		/*EnemyIsDead = true;*/
		Health = 0.0f;
		Die();
	}
	return DamageAmount;
	
	
}

void ANPC::Die()
{
	EnemyIsDead = true;
	UAnimInstance* AnimInstance = GetNPCMesh()->GetAnimInstance();
	//UAnimInstance* AnimInstance = this->GetNPCMesh()->GetAnimInstance();
	if (DeathAnimation != nullptr)
	{
			AnimInstance->Montage_Play(DeathAnimation, 1.0f);
	}
	
	
	//stop movement to play animation
	/*ANPC* Character = Cast<ANPC>(OtherActor);
	if (Character != nullptr)
	{
		if (Character)
		{
			UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
			if (GetMovementComponent)
			{
				GetMovementComponent->StopMovementImmediately();
			}
		}
	}*/

	ACharacter* NPC = this;
	FVector Location = GetActorLocation();
	//FVector Location = GetActorLocation();


	
	

}


USkeletalMeshComponent* ANPC::GetNPCMesh() const
{
	return GetMesh();
}





