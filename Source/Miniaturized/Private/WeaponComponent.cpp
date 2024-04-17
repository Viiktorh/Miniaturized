// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KismetTraceUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ProfilingDebugging/CookStats.h"


UWeaponComponent::UWeaponComponent()
{
	GuntipOffset = FVector(100.f, 10.f, 10.f);

	
}

void UWeaponComponent::AttachComponentToPlayer(APlayerCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	if (Character == nullptr || Character->GetHasWeapon())
	{
		return;
	}
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMeshPlayerCharacter(), AttachmentRules, FName(TEXT("AttachSocket")));

	Character->SetHasWeapon(true);

		//Adding the Input Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FireIMC, 1);
		}
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);

		if (EnhancedInputComponent)
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UWeaponComponent::FireWeapon);
		}
	}
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}
}

void UWeaponComponent::FireWeapon()
{
	FHitResult OutHit;

	//Determines where the Beam start and end locations.
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	FRotator BeamRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	FVector BeamStart = Character->GetActorLocation() + BeamRotation.RotateVector(GuntipOffset);
	FVector ForwardVector = PlayerController->PlayerCameraManager->GetActorForwardVector();
	FVector BeamEnd = ((ForwardVector * 1800.f) + BeamStart);
	//Initialize Collision parameters
	FCollisionQueryParams CollisionParams;
	

	

	GetWorld()->LineTraceSingleByChannel(OutHit, BeamStart, BeamEnd, TraceChannelProperty, CollisionParams);

	//Draws a thick blue line if the trace hits something, a thick red line if nothing is hit.
	DrawDebugLine(GetWorld(), BeamStart, BeamEnd, OutHit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
	UE_LOG(LogTemp, Log, TEXT("Tracing line: %s to %s"), *BeamStart.ToCompactString(), *BeamEnd.ToCompactString());


	// If the trace hit something, bBlockingHit will be true,
// and its fields will be filled with detailed info about what was hit
	if (OutHit.bBlockingHit && IsValid(OutHit.GetActor()))
	{
		UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *OutHit.GetActor()->GetName());
		if (OutHit.GetActor()->ActorHasTag("Enemy"))
		{
			UGameplayStatics::ApplyDamage(OutHit.GetActor(), 0.1f, PlayerController, Character, DamageType);
			//Draw a thin green line if the actor hit is Enemy
			DrawDebugLine(GetWorld(), BeamStart, BeamEnd, FColor::Green, false, 5.f, 5);
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("No Actors were hit"));
	}

	// Play Sound
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	//Play Animation
	if (FireAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = Character->GetMeshPlayerCharacter()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.0f);
		}
	}
}
