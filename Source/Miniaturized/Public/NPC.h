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

	//////////////////////////////////////////////////////////Bia start 
	/*Health*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float MaxHealth;

	

	//////////////////////////////////////////////////////////Bia end 
	//////////// Bia&Viktor? 
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
};
