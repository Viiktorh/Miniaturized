// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Rotate the character with the camera left/right rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	/*Spring Arm Component*/
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 600.0f; // The camera follows at this distance behind the character	
	CameraSpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	/*Camera Component*/
	PrimaryCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	PrimaryCameraComponent->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	PrimaryCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	/*Skeletal Mesh Component*/
	PlayerCharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharMesh"));
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Emerald, TEXT("Triggering the move Function"));

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
		AddControllerPitchInput(LookAroundVector.Y);
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

	//Adding the Input Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC, 0);
		}
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

