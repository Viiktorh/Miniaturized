
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
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
	SecondCameraComponent->Deactivate();//Deactivated by default

	/*Skeletal Mesh Component*/
	PlayerCharacterMesh = GetMesh();
	PlayerCharacterMesh->SetGenerateOverlapEvents(true);//Must be true for trigger to work properly

	/*Game save*/
	SaveObject = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));
	LoadObject = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));

	//For physics handle
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
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

void APlayerCharacter::Save()
{
	SaveObject->PlayerLocation = GetActorLocation();
	SaveObject->PlayerRotator = GetActorRotation();
	UGameplayStatics::SaveGameToSlot(SaveObject, TEXT("Slot1"), 0);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("Data saved ... "));
}

void APlayerCharacter::Load()
{
	LoadObject = UGameplayStatics::LoadGameFromSlot(TEXT("Slot1"), 0);
	if (!LoadObject)
	{
		Save();
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("No load, created load "));
	}
	SetActorLocation(SaveObject->PlayerLocation);
	SetActorRotation(SaveObject->PlayerRotator);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT(" Loaded."));
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

	//Shows the line and ensure it works
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Green : FColor::Magenta, false, 5.0f, 0, 10.0f);
	// UE_LOG(LogTemp, Log, TEXT("Tracing line: %s to %s"), *TraceStart.ToCompactString(), *TraceEnd.ToCompactString());
	if (Hit.bBlockingHit)
	{
		if (!Hit.GetActor()->Tags.IsEmpty() && IsValid(Hit.GetActor()))
		{

			//TODO: Show UI that says that its pushable
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("Pushable object"));
			IsPushable = true;
		}
	}
	else
	{
		IsPushable = false;
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("NOT PUSHABle"));
	}
}

void APlayerCharacter::Push()
{
	//if component grabbed
	if (PhysicsHandle->GrabbedComponent)
	{

		//Move the object we're holding each frame
		PhysicsHandle->SetTargetLocation(FVector(GetActorLocation().X + GetActorForwardVector().X * 100, GetActorLocation().Y + GetActorForwardVector().Y * 100, Hit.GetActor()->GetActorLocation().Z));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Error in grabbing object"));
	}
}

void APlayerCharacter::Grab()
{
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Display, TEXT("No physicshandle"));
		return;
	}

	if (!IsGrabbing && Hit.GetActor()->Tags[0] == "PushableObject")
	{
		UE_LOG(LogTemp, Display, TEXT("Hit actor %s"), *Hit.GetActor()->GetName());
		PhysicsHandle->GrabComponentAtLocationWithRotation(Hit.GetComponent(), NAME_None, Hit.GetComponent()->GetOwner()->GetActorLocation(), FRotator(0, 0, 0));
		IsGrabbing = true;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Nothing to grab or already grabbed an actor"));
	}
}

void APlayerCharacter::PushableObject()
{
	FVector Vel = this->GetVelocity();
	FVector Forward = this->GetActorForwardVector();
	FVector Right = this->GetActorRightVector();
	float ForwardSpeed = FVector::DotProduct(Vel, Forward);
	float RightSpeed = FVector::DotProduct(Vel, Right);

	if (!IsPushable) { return; }
	//Run different animation depending on speed
	if (ForwardSpeed > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("Pushing forward"));
	}
	if (ForwardSpeed < 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("Pushing Backward"));
	}
	//TODO: Different animation on direction
	Grab();
	Push();
}

void APlayerCharacter::ReleaseGrabbedObject()
{
	if (!IsGrabbing)
	{
		UE_LOG(LogTemp, Display, TEXT("Nothing grabbed"));
		return;
	}
	//TODO: Do we need to return to normal animation after changing?
	PhysicsHandle->ReleaseComponent();
	IsGrabbing = false;
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


	/*Respawn and load slot is set*/
	Save();
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
	Load();
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
	LineTrace(TraceDistance, TraceObject);
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
		EnhancedInputComponent->BindAction(PushObject, ETriggerEvent::Completed, this, &APlayerCharacter::ReleaseGrabbedObject);
		EnhancedInputComponent->BindAction(PushObject, ETriggerEvent::Triggered, this, &APlayerCharacter::PushableObject);
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
void APlayerCharacter::RunOnTagOverlap(FString Tag)
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
	if (Tag == "Checkpoint")
	{
		Save();
	}
	
}
//Called by trigger
void APlayerCharacter::RunOnTagEndOverlap(FString Tag)
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