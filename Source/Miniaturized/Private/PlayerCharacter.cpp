
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Rotate the character with the left/right rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	/*Spring Arm Component*/
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = StartSpringArmDistance; // The camera follows at this distance behind the character	
	CameraSpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraSpringArm->bEnableCameraLag = true;//Makes the camera movement feel smoother

	/*Second Springarm Component*/
	SecondSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SecondCameraBoom"));
	SecondSpringArm->SetupAttachment(RootComponent);
	SecondSpringArm->TargetArmLength = SecondSpringArmDistance;
	SecondSpringArm->bUsePawnControlRotation = true;
	SecondSpringArm->bEnableCameraLag = true;

	/*Camera Component*/
	PrimaryCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	PrimaryCameraComponent->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	PrimaryCameraComponent->AttachToComponent(CameraSpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	PrimaryCameraComponent->bUsePawnControlRotation = true;


	/*Health*/
	Health = 1.0f;
	RespawnLocation = FVector(0.0f, 0.0f, 0.0f);
	RespawnDelay = 5.0f;

	/*Weapon and ammo*/
	CurrentAmmo=0.0f;
	Min_Ammo=0.0f;
	Max_Ammo=3.0f;
	BatteryChargeDelay = 3.0f;

	/*Second camera component*/
	SecondCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SecondCameraComponent"));
	SecondCameraComponent->SetupAttachment(SecondSpringArm, USpringArmComponent::SocketName);
	SecondCameraComponent->bUsePawnControlRotation = false;
	SecondCameraComponent->AttachToComponent(SecondSpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	//Deactivated by default
	SecondCameraComponent->Deactivate();

	/*Skeletal Mesh Component*/
	PlayerCharacterMesh = GetMesh();
	PlayerCharacterMesh->SetGenerateOverlapEvents(true);//Must be true for trigger to work properly
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.3f, FColor::Emerald, TEXT("Triggering the move Function"));

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APlayerCharacter::LookAround(const FInputActionValue& Value)
{
	FVector2D LookAroundVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAroundVector.X);
		AddControllerPitchInput((LookAroundVector.Y));
	}
}


void APlayerCharacter::SetHasWeapon(bool bHasNewWeapon)
{
	bHasWeapon = bHasNewWeapon;
}

bool APlayerCharacter::GetHasWeapon()
{
	return bHasWeapon;
}

USkeletalMeshComponent* APlayerCharacter::GetMeshPlayerCharacter() const
{
	return PlayerCharacterMesh;
}

UCameraComponent* APlayerCharacter::GetPrimaryCameraComponent() const
{
	return PrimaryCameraComponent;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SpringArmStartRotation = CameraSpringArm->GetRelativeRotation();
	PlayerController = Cast<APlayerController>(Controller);
	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	//Adding the Input Context
	if (PlayerController)
		{
			if (Subsystem)
				{
					Subsystem->AddMappingContext(IMC, 0);
				}
		}


	/*Respawn. Location resets */
	RespawnLocation = GetActorLocation();
}

/*float decided in blueprint*/
void APlayerCharacter::TakeDamage(float DamageDealt)
{
	Health -= DamageDealt;
	if (Health <= 0.0f) {
		Die();
	}
}

/*float decided in blueprint*/
void APlayerCharacter::Heal(float HealingRestored)
{
	Health += HealingRestored;
	if (Health >= 1.0f) {
		Health = 1.0f;
	}
}

/*calls respawn function with delay so the animation can be played later*/
void APlayerCharacter::Die()
{
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &APlayerCharacter::Respawn, RespawnDelay, false);
}

/*resets health, location and timer */
void APlayerCharacter::Respawn()
{
	Health = 1.0f;
	SetActorLocation(RespawnLocation);
	GetWorldTimerManager().ClearTimer(RespawnTimerHandle);
}

void APlayerCharacter::GetAmmo(float CollectedAmmo)
{
	CurrentAmmo += CollectedAmmo;
	if (CurrentAmmo >= Max_Ammo) {
		CurrentAmmo = 3.0f;
	}
	GetWorld()->GetTimerManager().SetTimer(BatteryChargeHandle, this, &APlayerCharacter::LoosingCharge, BatteryChargeDelay, true);

}

void APlayerCharacter::LoosingCharge()
{
	if (CurrentAmmo > Min_Ammo) {
		CurrentAmmo -= 0.3f;
	}
	
	else {
		CurrentAmmo = 0.0f;
		GetWorldTimerManager().ClearTimer(BatteryChargeHandle);
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LookAround);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}
}


void APlayerCharacter::ChangeSpringarmWithTimer()
{
	if (CameraSpringArm->GetRelativeRotation().Yaw <= -89)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		UE_LOG(LogTemp, Warning, TEXT(" Pitch is: %f, Yaw is: %f and Roll is: %f"), CameraSpringArm->GetRelativeRotation().Pitch, CameraSpringArm->GetRelativeRotation().Yaw, CameraSpringArm->GetRelativeRotation().Roll);
		UE_LOG(LogTemp, Warning, TEXT("Timer Cleared"))
	}
	//Rotates and increases the springarm location relative to mesh, turns off springarm collision
	CameraSpringArm->bDoCollisionTest = false;
	CameraSpringArm->bUsePawnControlRotation = false;
	CameraSpringArm->TargetArmLength = FMath::FInterpTo(CameraSpringArm->TargetArmLength, SideViewSpringArmDistance, GetWorld()->GetDeltaSeconds(), SideViewIntSpeed);
	CameraSpringArm->SetRelativeRotation(FMath::RInterpTo(CameraSpringArm->GetRelativeRotation(), SideViewRotation, GetWorld()->GetDeltaSeconds(), SideViewIntSpeed));
}
void APlayerCharacter::ReturnSpringarmWithTimer()
{
	if (CameraSpringArm->GetRelativeRotation().Yaw >= -1)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("Pitch is: %f, Yaw is: %f and Roll is: %f"), CameraSpringArm->GetRelativeRotation().Pitch, CameraSpringArm->GetRelativeRotation().Yaw, CameraSpringArm->GetRelativeRotation().Roll);

		UE_LOG(LogTemp, Warning, TEXT("Timer Cleared"))
	}

	//Rotates and increases the springarm location relative to mesh, returns collision to true
	CameraSpringArm->SetRelativeRotation(FMath::RInterpTo(CameraSpringArm->GetRelativeRotation(), SpringArmStartRotation, GetWorld()->GetDeltaSeconds(), SideViewIntSpeed));
	CameraSpringArm->TargetArmLength = FMath::FInterpTo(CameraSpringArm->TargetArmLength, StartSpringArmDistance, GetWorld()->GetDeltaSeconds(), SideViewIntSpeed);
	CameraSpringArm->bUsePawnControlRotation = true;
	CameraSpringArm->bDoCollisionTest = true;

}

void APlayerCharacter::SwitchToTerrariumImc() const
{
	if (PlayerController)
	{
		if (Subsystem)
		{
			Subsystem->RemoveMappingContext(IMC);
			Subsystem->AddMappingContext(IMC_Terrarium, 0);
		}
	}
}

void APlayerCharacter::SwitchToDefaultImc() const
{
	if (PlayerController)
	{
		if (Subsystem)
		{
			Subsystem->RemoveMappingContext(IMC_Terrarium);
			Subsystem->AddMappingContext(IMC, 0);
		}
	}
}

//Called by trigger
void APlayerCharacter::TurnToDifferentView(FString Tag)
{
	if (Tag.Len() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Tag found, add tag to trigger"));
	}

	if (Tag == "Terrarium")
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::ChangeSpringarmWithTimer, Delay, true);
		SwitchToTerrariumImc();
	}

	if (Tag == "InnerTerrarium")
	{
		SwitchToDefaultImc();
		SecondCameraComponent->Activate();
		PrimaryCameraComponent->Deactivate();
	}

}
//Called by trigger
void APlayerCharacter::ReturnSpringarmToDefault(FString Tag)
{
	if (Tag.Len() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Tag found, add tag to trigger"));
	}
	if (Tag == "Terrarium")
	{
		//Return control and camera to default
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::ReturnSpringarmWithTimer, Delay, true);
		SwitchToDefaultImc();

	}
	if (Tag == "InnerTerrarium")
	{
		SwitchToTerrariumImc();
		SecondCameraComponent->Deactivate();
		PrimaryCameraComponent->Activate();
	}
}