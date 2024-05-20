  
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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
	CameraSpringArm->TargetArmLength = StartSpringArmDistance; // The  camera follows at this distance behind the character	
	CameraSpringArm->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraSpringArm->bEnableCameraLag = true;//Makes the camera movement feel smoother

	/*Camera Component*/
	PrimaryCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	PrimaryCameraComponent->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	PrimaryCameraComponent->AttachToComponent(CameraSpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	PrimaryCameraComponent->bUsePawnControlRotation = true;


	/*Health*/
	MaxHealth = 100.0f;
	Health = MaxHealth;
	RespawnDelay = 5.0f;
	bIsEndOfGame = false;

	/*Weapon and ammo*/
	CurrentAmmo=0.0f;
	Min_Ammo=0.0f;
	Max_Ammo=25.0f;
	BatteryChargeDelay = 3.0f;

	/*Vials*/
	CurrentVials = 0.0f;
	Min_Vials = 0.0f;
	Max_Vials = 3.0f;


	/*Skeletal Mesh Component*/
	PlayerCharacterMesh = GetMesh();
	PlayerCharacterMesh->SetGenerateOverlapEvents(true);//Must be true for trigger to work properly

	
	//For physics handle
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	if (GetMovementComponent())
	{
		GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LineTrace(TraceDistance, TraceObject);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{

	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}
void APlayerCharacter::MoveTerrarium(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::LookAround(const FInputActionValue& Value)
{
	FVector2D LookAroundVector = Value.Get<FVector2D>();

	if (Controller != nullptr && !IsGrabbing)
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

void APlayerCharacter::SetCurrentAmmo(float AmmoChange)
{
	CurrentAmmo = AmmoChange;
}

float APlayerCharacter::GetCurrentAmmo()
{
	return CurrentAmmo;
}


void APlayerCharacter::Save()
{
	SaveObject = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));
	SaveObject->PlayerLocation = GetActorLocation();
	SaveObject->PlayerRotator = GetActorRotation();
	
	UGameplayStatics::SaveGameToSlot(SaveObject, TEXT("Slot1"), 0);
	//Left in on purpose, for the player to see that progress is saved
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("Data saved ... "));
}

void APlayerCharacter::Load()
{
	SetActorLocation(SaveObject->PlayerLocation);
	SetActorRotation(SaveObject->PlayerRotator);
	Health = MaxHealth;
	CurrentAmmo = Max_Ammo;
	HasRespawned = true;
}

void APlayerCharacter::LineTrace(float LineDistance, TEnumAsByte<ECollisionChannel> TraceChannel)
{
	// where the trace starts and how far it goes
	TraceStart = GetActorLocation();
	TraceEnd = GetActorLocation() + GetActorForwardVector() * LineDistance;

	// ignores this actor as to not block the trace
	QueryParams.AddIgnoredActor(this);
	//Runs a trace and return first actor hit within the channel to "Hit"
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannel, QueryParams);

	////Shows the line for debugging
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Green : FColor::Magenta, false, 5.0f, 0, 10.0f);
	if (Hit.bBlockingHit)
	{
		if (!Hit.GetActor()->Tags.IsEmpty() && IsValid(Hit.GetActor()))
		{
			if (Hit.GetActor()->Tags[0] == "PushableObject")
			{
				IsPushable = true;
			}
		}
	}
	else
	{
		IsPushable = false;
		ReleaseGrabbedObject();
	}
}

void APlayerCharacter::Push()
{
	//if component grabbed
	if (PhysicsHandle->GrabbedComponent)
	{
		//Move the object grabbed in physicshandle
		PhysicsHandle->SetTargetLocation(FVector(GetActorLocation().X + GetActorForwardVector().X * 100,
		                                         GetActorLocation().Y + GetActorForwardVector().Y * 100,
		                                         Hit.GetComponent()->GetOwner()->GetActorLocation().Z));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No object grabbed"));
	}
}

void APlayerCharacter::Grab()
{
	if (!PhysicsHandle){ return; }

	//Grab the object and set slightly in front of the player
	FVector GrabLocation = FVector(Hit.ImpactPoint.X, Hit.ImpactPoint.Y, Hit.GetComponent()->GetOwner()->GetActorLocation().Z);
	PhysicsHandle->GrabComponentAtLocationWithRotation(Hit.GetComponent(), NAME_None, GrabLocation,
	                                                   Hit.GetComponent()->GetOwner()->GetActorRotation());
	IsGrabbing = true;
	
}

void APlayerCharacter::PushableObject()
{
	if (!IsPushable) { return; }

	if (IsGrabbing) { Push(); }
	 else { Grab(); }
}

void APlayerCharacter::ReleaseGrabbedObject()
{
	if (!IsGrabbing) { return; }
	
	PhysicsHandle->ReleaseComponent();
	IsGrabbing = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*Game save*/
	SaveObject = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));

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
	/*Respawn and load slot is set at start of game*/
	Save();
}




float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;
	if (Health <= 0.0f) {
		Die();
	}
	return DamageAmount;
}

/*float decided in blueprint*/
void APlayerCharacter::Heal(float HealingRestored)
{
	Health += HealingRestored;
	if (Health >= 100.0f) {
		Health = 100.0f;
	}
}

/*calls respawn function with delay so the animation can be played later*/
void APlayerCharacter::Die()
{
	bTimerHasStarted = false;
	Respawn();
}

/*resets health, location and timer */
void APlayerCharacter::Respawn()
{
	Load();
}

void APlayerCharacter::CollectAmmo(float CollectedAmmo)
{
	CurrentAmmo += CollectedAmmo;
	if (CurrentAmmo >= Max_Ammo) {
		CurrentAmmo = Max_Ammo;
	}
	GetWorld()->GetTimerManager().SetTimer(BatteryChargeHandle, this, &APlayerCharacter::GainCharge, BatteryChargeDelay, true);

}

void APlayerCharacter::GainCharge()
{
	if (CurrentAmmo < BatteryRechargeLimit) {
		CurrentAmmo += BatteryRechargeRate;
	}
}

void APlayerCharacter::GetVials(float CollectedVials)
{
	CurrentVials += CollectedVials;
	if (CurrentVials >= Max_Vials) {
		CurrentVials = 3.0f;
	}
}


// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(MoveActionTerrarium, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveTerrarium);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LookAround);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(PushObject, ETriggerEvent::Completed, this, &APlayerCharacter::ReleaseGrabbedObject);
		EnhancedInputComponent->BindAction(PushObject, ETriggerEvent::Triggered, this, &APlayerCharacter::PushableObject);
	}
}

void APlayerCharacter::SwitchToTerrariumImc()
{
	if (PlayerController)
	{
		if (Subsystem)
		{
			Subsystem->RemoveMappingContext(IMC);
			Subsystem->AddMappingContext(IMC_Terrarium, 0);
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
			bUseControllerRotationYaw = false;
			
		}
	}
}

void APlayerCharacter::SwitchToDefaultImc()
{
	if (PlayerController)
	{
		if (Subsystem)
		{
			Subsystem->RemoveMappingContext(IMC_Terrarium);
			Subsystem->AddMappingContext(IMC, 0);
			GetCharacterMovement()->bOrientRotationToMovement = false;
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
			bUseControllerRotationYaw = true;
			
		}
	}
}

void APlayerCharacter::JumpPad(double Forward, double HowHigh)
{
	LaunchCharacter(FVector(0,Forward,HowHigh),false,false);
}

//Called by trigger
void APlayerCharacter::RunOnTagOverlap(FString Tag)
{
	if (Tag.Len() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Tag found, add tag to trigger"));
	}

	if (Tag == "Checkpoint") { Save();}
}
