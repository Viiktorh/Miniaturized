// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"



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

	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	//Determines where the Beam start and end locations are.
	FRotator BeamRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	FVector BeamStart = Character->GetActorLocation() + BeamRotation.RotateVector(GuntipOffset);
	FVector ForwardVector = PlayerController->PlayerCameraManager->GetActorForwardVector();
	FVector BeamEnd = ((ForwardVector * 1800.f) + BeamStart);
	//Initialize Collision parameters
	FCollisionQueryParams CollisionParams;

	
	//Execute a LineTrace with OutHit as result
	GetWorld()->LineTraceSingleByChannel(OutHit, BeamStart, BeamEnd, TraceChannelProperty, CollisionParams);
	


	////Draws a thick blue line if the trace hits something, a thick red line if nothing is hit.
	//DrawDebugLine(GetWorld(), BeamStart, BeamEnd, OutHit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
	//UE_LOG(LogTemp, Log, TEXT("Tracing line: %s to %s"), *BeamStart.ToCompactString(), *BeamEnd.ToCompactString());


	// If the trace hit something, bBlockingHit will be true,
	// and its fields will be filled with detailed info about what was hit
	if (OutHit.bBlockingHit && IsValid(OutHit.GetActor()))
	{
		//Debug
		UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *OutHit.GetActor()->GetName());
		//Do this before applying damage or else risk writing to a nullptr when enemies die.
		if (OutHit.GetActor()->ActorHasTag("Enemy") && OutHit.GetActor() != nullptr)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeaponBeam, BeamStart);
			if (NiagaraComp)
			{
				//BeamTarget is the end of the beam. Defined from the NS_WeaponBeam Niagara Comp in the unreal editor.
				NiagaraComp->SetVectorParameter(FName("BeamTarget"), OutHit.GetActor()->GetActorLocation());
			}
		}
		//Apply damage if Actor is Enemy and draw a thin green line.
		if (OutHit.GetActor()->ActorHasTag("Enemy"))
		{
			UGameplayStatics::ApplyDamage(OutHit.GetActor(), 0.1f, PlayerController, Character, DamageType);
			//Draw a thin green line if the actor hit is Enemy
			DrawDebugLine(GetWorld(), BeamStart, BeamEnd, FColor::Green, false, 5.f, 5);
			UGameplayStatics::ApplyDamage(OutHit.GetActor(), WeaponDamage, PlayerController, Character, DamageType);
			/*DrawDebugLine(GetWorld(), BeamStart, BeamEnd, FColor::Green, false, 5.f, 5);*/
		}
		else 
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeaponBeam, BeamStart);
			/*UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(WeaponBeam, OutHit.GetComponent(), FName("BeamTarget"), Character->GetActorLocation(), Character->GetActorRotation(), EAttachLocation::SnapToTarget, true);*/
			if (NiagaraComp)
			{
				//BeamTarget is the end of the beam. Defined from the NS_WeaponBeam Niagara Comp in the unreal editor.
				NiagaraComp->SetVectorParameter(FName("BeamTarget"), OutHit.Location);
			}
		}
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("No Actors were hit"));

		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeaponBeam, BeamStart);
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

//UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), WeaponBeam, BeamStart);
