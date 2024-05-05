// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealingObject.h"
#include "NPC.generated.h"

UCLASS()
class MINIATURIZED_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//USkeletalMeshComponent* NPCMesh=GetMesh();

	//////////////////////////////////////////////////////////
	/*Health*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool EnemyIsDead;

	//////////////////////////////////////////////////////////
	//////////// 
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	)override;

	UFUNCTION()
	void Die();

	FTimerHandle DieTimer;

	UPROPERTY()
	float DieDelay;

	//animation 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* DeathAnimation;

	USkeletalMeshComponent* GetNPCMesh() const;

	//UFUNCTION(BlueprintCallable, Category = "Components|Animation", meta = (Keywords = "Animation", UnsafeDuringActorConstruction = "true"))
	//void PlayAnimation(class UAnimationAsset* DeathAnimation, bool bLooping);

	//bool bLooping = true;
	//USkeletalMeshComponent* GetMesh() const;

	/*FVector EnemyLocation;
	FRotator EnemyRotation;
	FActorSpawnParameters SpawnInfo;
	*/
};
