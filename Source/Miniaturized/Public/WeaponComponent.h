// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Miniaturized/Public/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "WeaponComponent.generated.h"

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

	/*
	 * IMC and Actions
	 */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* FireIMC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* FireAction;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void FireWeapon();

	/*
	 * Animation and Sound
	 */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation and Sound")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation and Sound")
	UAnimMontage* FireAnimation;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Electricity Beam")

};