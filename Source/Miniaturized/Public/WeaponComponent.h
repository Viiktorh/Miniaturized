// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Miniaturized/Public/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "WeaponComponent.generated.h"

class UParticleEmitter;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class MINIATURIZED_API UWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

	APlayerCharacter* Character;

	UFUNCTION(Blueprintable, BlueprintCallable, Category = "WeaponModel")
	void AttachComponentToPlayer(APlayerCharacter* TargetCharacter);

	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSettings")
	FVector GuntipOffset;


	UPROPERTY(EditAnywhere, Category = "WeaponSettings")
	float WeaponDamage = 10.f;

	UPROPERTY(VisibleAnywhere = "WeaponSettings")
	float WeaponAmmoConsumption = 0.0166f; //0.0166 is ~ 1 ammo per second.

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> DamageType;


	/*
	 * Expose Collision Channel to Unreal Editor
	 */
	UPROPERTY(EditAnywhere, Category = "Beam Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Visibility;

	/*
	 * IMC and Actions
	 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* FireIMC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* FireTerrariumIMC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* FireAction;

	UFUNCTION(BlueprintCallable, Category = "Weapon function")
	void FireWeapon();

	/*
	 * Animation and Sound
	 */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation and Sound")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation and Sound")
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation and Sound")
	UNiagaraSystem* WeaponBeam;

	

};