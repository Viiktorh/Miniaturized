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

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


float ANPC::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,AActor* DamageCauser)
{
	Health -= DamageAmount;
	if(Health <= 0.0f)
	{
		Health = 0.0f;
		Die();
	}
	return DamageAmount;
}

void ANPC::Die()
{
	EnemyIsDead = true;
	if (DeathAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = this->GetNPCMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(DeathAnimation, 1.0f);
		}
	}
	//Destroy();
	//PlayAnimation(DeathAnimation,false);
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
	FVector Location = NPC->GetActorLocation();
	//FVector Location = GetActorLocation();
	float SpawnX = FMath::RandRange(Location.X + 10.0f, Location.X + 10.0f);
	float SpawnY = FMath::RandRange(Location.Y + 10.0f, Location.Y + 10.0f);
	float SpawnZ = FMath::RandRange(Location.Z + 1.0f, Location.Z + 10.0f);
	FVector RespawnLocation = FVector(SpawnX, SpawnY, SpawnZ);

	
	
	AHealingObject* SpawnedHealingObject = GetWorld()->SpawnActor<AHealingObject>(AHealingObject::StaticClass(), RespawnLocation, FRotator::ZeroRotator);
	
	
	

}

USkeletalMeshComponent* ANPC::GetNPCMesh() const
{
	return GetMesh();
}


/*void USkeletalMeshComponent::PlayAnimation(class UAnimationAsset* DeathAnimation, bool bLooping)
{
	SetAnimationMode(EAnimationMode::AnimationSingleNode);
	SetAnimation(DeathAnimation);
	Play(bLooping);
}

/*USkeletalMeshComponent* ANPC::GetMesh() const
{
	//USkeletalMeshComponent* NPCMesh = <USkeletalMesh>(this, TEXT("NPCMesh"));
	return NPCMesh;
}*/



