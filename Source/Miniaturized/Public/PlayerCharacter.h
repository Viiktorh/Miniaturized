// Fill out your copyright notice in the Description page of Project Settings.
//Test
#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UCameraComponent;
class UInputAction;
class InputComponent;
class InputAction;

UCLASS()
class MINIATURIZED_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*
	* Components
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* PrimaryCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* CameraSpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* PlayerCharacterMesh;

	/*
	 * Input Mapping Context and Actions
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

	void Move(const FInputActionValue& Value);

	void LookAround(const FInputActionValue& Value);

	/*
	* Weapon
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bHasWeapon = false;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetHasWeapon(bool bHasNewWeapon);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool GetHasWeapon();

	USkeletalMeshComponent* GetMeshPlayerCharacter() const;

	UCameraComponent* GetPrimaryCameraComponent() const;

	/*Health*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float Health;

	/*Respawn*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
	FVector RespawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
	float RespawnDelay;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*Health and respawn*/

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float DamageDealt);

	UFUNCTION(BlueprintCallable)
	void Heal(float HealingRestored);

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void Respawn();

	FTimerHandle RespawnTimerHandle;

};
