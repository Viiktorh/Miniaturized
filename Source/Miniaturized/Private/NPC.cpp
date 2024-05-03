// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "NPC.h"

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
	//PlayAnimation(DeathAnimation,false);
	Destroy();
	

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



