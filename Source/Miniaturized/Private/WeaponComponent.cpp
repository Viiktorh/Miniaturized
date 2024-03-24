// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "KismetTraceUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ProfilingDebugging/CookStats.h"

UWeaponComponent::UWeaponComponent()
{
	GuntipOffset = FVector(100.f, 0.f, 10.f);
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
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	FVector BeamStart = Character->GetActorLocation();
	FRotator BeamRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	BeamStart.Z += 0.f;
	BeamStart.X += 0.f;
	FVector ForwardVector = PlayerController->PlayerCameraManager->GetActorForwardVector();
	FVector UpVector = PlayerController->PlayerCameraManager->GetActorUpVector();
	FVector End = ((ForwardVector * 1800.f) + BeamStart);
	FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), BeamStart, End, FColor::Blue, false, 1, 5);
	
	bool isHit = Character->ActorLineTraceSingle(OutHit, BeamStart, End, ECC_WorldStatic, CollisionParams);

	if(isHit)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Hitting Component: %s"), *OutHit.GetComponent()->GetName()));
		}
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
