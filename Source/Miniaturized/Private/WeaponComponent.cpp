// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/GameplayStatics.h"



UWeaponComponent::UWeaponComponent()
{
	GuntipOffset = FVector();
	GuntipOffset = USkeletalMeshComponent::GetSocketLocation(FName(TEXT("BeamSocket")));

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
	if (Character->GetCurrentAmmo() <= Character->Min_Ammo)
	{
		Character->SetCurrentAmmo(Character->Min_Ammo);
		return;
	}
	if (Character->GetCurrentAmmo() >= Character->Min_Ammo)
	{
		float AmmoChange = Character->GetCurrentAmmo() - .1f;
		Character->SetCurrentAmmo(AmmoChange);
	}

	FHitResult OutHit;

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	//Determines where the Beam start and end locations are.
	FRotator BeamRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	FVector BeamStart = Character->GetActorLocation() + BeamRotation.RotateVector(GuntipOffset);
	FVector ForwardVector = PlayerController->PlayerCameraManager->GetActorForwardVector();
	FVector BeamEnd = ((ForwardVector * 1800.f) + BeamStart);
	//Initialize Collision parameters
	FCollisionQueryParams CollisionParams;

	//Use Different LineTrace Parameters if the Character has the Terrarium IMC
	if(Character->Subsystem->HasMappingContext(Character->IMC_Terrarium))
	{
		BeamRotation = PlayerController->GetControlRotation();
		BeamStart = Character->GetActorLocation() + BeamRotation.RotateVector(GuntipOffset);
		ForwardVector = Character->GetActorForwardVector();
		BeamEnd = ((ForwardVector * 1800.f) + BeamStart);
	}
	
	//Execute a LineTrace with OutHit as result
	GetWorld()->LineTraceSingleByChannel(OutHit, BeamStart, BeamEnd, TraceChannelProperty, CollisionParams);

	// If the trace hit something, bBlockingHit will be true,
	// and its fields will be filled with detailed info about what was hit
	if (OutHit.bBlockingHit && IsValid(OutHit.GetActor()))
	{
		//Attach the beam to the enemy
		//Do this before applying damage or else risk checking a nullptr when enemies die.
		if (OutHit.GetActor()->ActorHasTag("Enemy") && OutHit.GetActor() != nullptr)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeaponBeam, USkeletalMeshComponent::GetSocketLocation(FName(TEXT("BeamSocket"))));
			if (NiagaraComp)
			{
				//Sets the BeamTarget at the Actor Location
				//BeamTarget is the end of the beam. Defined from the NS_WeaponBeam Niagara Comp in the unreal editor.
				NiagaraComp->SetVectorParameter(FName("BeamTarget"), OutHit.GetActor()->GetActorLocation());
			}
		}
		//Apply damage if Actor is Enemy.
		if (OutHit.GetActor()->ActorHasTag("Enemy"))
		{
			UGameplayStatics::ApplyDamage(OutHit.GetActor(), WeaponDamage, PlayerController, Character, DamageType);
		}
		else 
		{
			//If something other than enemy is hit, attach beam to the hit location
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeaponBeam, USkeletalMeshComponent::GetSocketLocation(FName(TEXT("BeamSocket"))));
			if (NiagaraComp)
			{
				//BeamTarget is the end of the beam. Defined from the NS_WeaponBeam Niagara Comp in the unreal editor.
				NiagaraComp->SetVectorParameter(FName("BeamTarget"), OutHit.Location);
			}
		}
	}
	else {
		//Attach the beam to the BeamEnd location
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeaponBeam, USkeletalMeshComponent::GetSocketLocation(FName(TEXT("BeamSocket"))));
		if (NiagaraComp)
		{
			//BeamTarget is the end of the beam. Defined from the NS_WeaponBeam Niagara Comp in the unreal editor.
			NiagaraComp->SetVectorParameter(FName("BeamTarget"), BeamEnd);
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
	if (WeaponBeam != nullptr)
	{
		return;
	}
}
