// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraSpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* PlayerCharacterMesh;

	/*
	 * Input Mapping Context and Actions
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* IMC_Terrarium;//Specifically for the terrarium section

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

	void Move(const FInputActionValue& Value);

	void LookAround(const FInputActionValue& Value);

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraSpringArm; }

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


	/*
	 *Terrarium camera and control
	 */
	 //Springarm start position is saved at the start
	FRotator SpringArmStartRotation;

	 //Player controller and subsystem
	APlayerController* PlayerController;
	UEnhancedInputLocalPlayerSubsystem* Subsystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sideview position")
	FRotator SideViewRotation = FRotator(0, 0, -100);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sideview position")
	float StartSpringArmDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sideview position")
	float SideViewSpringArmDistance = 800.0f;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sideview position")
	float SideViewIntSpeed = 2.0f;//Value decides how fast the springarm turns when changing position & rotation

	//Timer 
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer delay")
	float Delay = 0.01f;

	//Repositions springarm and switches controls
	void TurnSpringarmToSideView();

	//Returns springarm and control to default
	void ReturnSpringarmToStartingPos();

	/*Rotates the springarm relative to its parent and increases the springarm target length.
	 *Turns off collision on the springarm. When it reaches its desired location, timer that is also used is cleared*/
	void ChangeSpringarmWithTimer();

	/*Rotates the springarm back to its start position relative to its parent and decreases the springarm target length.
	 *Turns on collision on the springarm. When it reaches its desired location, timer that is used is cleared*/
	void ReturnSpringarmWithTimer();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
