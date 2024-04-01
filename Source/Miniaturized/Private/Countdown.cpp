// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Engine/World.h"
#include "Components/TextRenderComponent.h"
#include "Countdown.h"

// Sets default values
ACountdown::ACountdown()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CountdownText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Mix will be ready in 2 minutes"));
	CountdownText->SetHorizontalAlignment(EHTA_Center);
	CountdownText->SetWorldSize(250.0f);
	CountdownText->SetTextRenderColor(FColor::Black);
	RootComponent = CountdownText;
	bCanPickupPotion = false;
}

// Called when the game starts or when spawned
void ACountdown::BeginPlay()
{
	Super::BeginPlay();

	/* UUserWidget* Countdown = CreateWidget<UUserWidget>(GetWorld(), YourWidgetClass);
    if (Countdown)
    {
        Countdown->AddToViewport();
    }*/

	/*test for beginning */

	CountdownText->SetText(FText::FromString(FormattedTime));
	GetWorld()->GetTimerManager().SetTimer(CountdownTimer, this, &ACountdown::CountdownBegin, 1.0f, true);
	UWorld* World = GetWorld();
	if (World) {
		UGameplayStatics::PlaySound2D(World, CountdownSound, 1.f, 1.f, 0.f);
	}
	
}

// Called every frame
void ACountdown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACountdown::CountdownBegin()
{
	Time-=1.0f;
	int32 Minutes = static_cast<int32>(Time) / 60;
	int32 Seconds = static_cast<int32>(Time) % 60;
	FormattedTime = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	if (Time <= 0) {
		CountdownFinished();
	}
}

void ACountdown::CountdownFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(CountdownTimer);
	Destroy();
	CountdownText->SetText(INVTEXT("You can collect potion!"));
	bCanPickupPotion = true;
	
	
}

