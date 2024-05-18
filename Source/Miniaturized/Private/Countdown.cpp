// Fill out your copyright notice in the Description page of Project Settings.


#include "Countdown.h"

// Sets default values
ACountdown::ACountdown()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CountdownText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	CountdownText->SetHorizontalAlignment(EHTA_Center);
	CountdownText->SetWorldSize(100.0f);
	CountdownText->SetTextRenderColor(FColor::White);
	CountdownText->SetText(FText::FromString("Press green button to start the mixer"));
	RootComponent = CountdownText;
	//bCanPickupPotion = false;
	/*trigger box to activate timer*/
	TriggerBoxTimer = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBoxTimer->SetupAttachment(RootComponent);
	bCollisionEnabled = true;

	

	
}

// Called when the game starts or when spawned
void ACountdown::BeginPlay()
{
	Super::BeginPlay();
	TriggerBoxTimer->OnComponentBeginOverlap.AddDynamic(this, &ACountdown::OnBoxBeginOverlap);
}

// Called every frame
void ACountdown::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACountdown::Countdown()
{
	Time -= 1.0f;
	int32 Minutes = static_cast<int32>(Time) / 60;
	int32 Seconds = static_cast<int32>(Time) % 60;
	FormattedTime = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	CountdownText->SetText(FText::FromString(FormattedTime));
	if (Time <= 0) {
		CountdownFinished();
	}
	if (Character->bTimerHasStarted == false) {
		ResetCountdown();
	}

}

void ACountdown::CountdownFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(CountdownTimer);
	CountdownText->SetText(INVTEXT("You can collect potion!"));
}

void ACountdown::ResetCountdown()
{
	GetWorld()->GetTimerManager().ClearTimer(CountdownTimer);
	CountdownText->SetText(FText::FromString("Press green button to start the mixer"));
	Time = 120.0f;
	bCollisionEnabled = true;
}


void ACountdown::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character = Cast<APlayerCharacter>(OtherActor);
	
	if (Character != nullptr) {
		
		float CheckCurrentVials = Character->CurrentVials;
		float MaxCurrentVials = Character->Max_Vials;

		if (CheckCurrentVials == MaxCurrentVials) {
			
			if (Character->bTimerHasStarted == false) {
				if (bCollisionEnabled) {
					GetWorld()->GetTimerManager().SetTimer(CountdownTimer, this, &ACountdown::Countdown, 1.0f, true);
					
					UWorld* World = GetWorld();
					if (World) {
						UGameplayStatics::PlaySound2D(World, CountdownSound, 1.f, 1.f, 0.f);
					}
					Character->bTimerHasStarted = true;
					bCollisionEnabled = false;
				}
			}
		}

		else {
			CountdownText->SetText(FText::FromString("You don't have enough vials "));
		}	
	}
}

void ACountdown::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CountdownFinished();
}


