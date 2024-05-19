// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystemInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MainSaveGame.h"
#include "cameraSwitchInterface.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UCameraComponent;
class UInputAction;
class InputComponent;
class InputAction;

UCLASS()
class MINIATURIZED_API APlayerCharacter : public ACharacter, public IcameraSwitchInterface
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FirstSpringarm")
	float StartSpringArmDistance = 500.0f;

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
	UInputAction* MoveActionTerrarium;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* PushObject;


	void Move(const FInputActionValue& Value);

	void MoveTerrarium(const FInputActionValue& Value);

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


	/*Ammo*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Min_Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Max_Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BatteryChargeDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BatteryRechargeRate = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BatteryRechargeLimit = 5.f;

	void SetCurrentAmmo(float AmmoChange);

	float GetCurrentAmmo();

	/*Vials*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vials")
	float CurrentVials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vials")
	float Min_Vials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vials")
	float Max_Vials;

	/*Health*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
	float RespawnDelay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn")
	bool bIsEndOfGame;

	bool HasRespawned = false;
	bool bTimerHasStarted = false;

	 /*GameSave*/
	UPROPERTY()
	UMainSaveGame *SaveObject;
	USaveGame *LoadObject;

	UFUNCTION(BlueprintCallable)
	void Save();
	UFUNCTION(BlueprintCallable)
	void Load();

	//Player controller and subsystem
	APlayerController* PlayerController;
	UEnhancedInputLocalPlayerSubsystem* Subsystem;


	//Timer 
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer delay")
	float Delay = 0.01f;

	//Checks for tags when overlaping and runs code accordingly
	virtual void RunOnTagOverlap(FString Tag) override;


	//Switch inputmapping
	void SwitchToTerrariumImc();
	void SwitchToDefaultImc();

	/*
	 *Spring/Jumpad
	 */
	//Jump as high as you want
	void JumpPad(double Forward, double HowHigh);

	/*
	 * Push and pull objects parameters
	 */
	FHitResult Hit;
	TEnumAsByte<ECollisionChannel> TraceObject = ECC_Pawn;
	FVector TraceStart;
	FVector TraceEnd;
	FCollisionQueryParams QueryParams;
	UPhysicsHandleComponent* PhysicsHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Traceline distance")
	float TraceDistance = 100.0f;

	bool IsGrabbing = false;
	bool  IsPushable;

	 //shoots a line in front of the character and saves what it hits to "Hit"
	void LineTrace(float LineDistance, TEnumAsByte<ECollisionChannel> TraceChannel);
	//Move object 
	void Push();
	//Use physicshandle to grab object 
	void Grab();
	//Decide whether an object is pushable and run push and grab to grab and push the object
	void PushableObject();
	//Release grabbed object
	void ReleaseGrabbedObject();


	/*overlap interact (currently: vent)*/
	/*UFUNCTION()
	void OnObjectBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*Health and respawn*/

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	)override;

	UFUNCTION(BlueprintCallable)
	void Heal(float HealingRestored);

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void Respawn();

	FTimerHandle RespawnTimerHandle;

	/*Ammo and shooting*/
	UFUNCTION(BlueprintCallable)
	void CollectAmmo(float CollectedAmmo);

	UFUNCTION(BlueprintCallable)
	void GainCharge();

	FTimerHandle BatteryChargeHandle;

	/*Viles*/
	UFUNCTION(BlueprintCallable)
	void GetVials(float CollectedVials);

};
