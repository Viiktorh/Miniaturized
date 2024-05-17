// Fill out your copyright notice in the Description page of Project Settings.


#include "Countdown.h"

// Sets default values
ACountdown::ACountdown()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CountdownText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	CountdownText->SetHorizontalAlignment(EHTA_Center);
	CountdownText->SetWorldSize(250.0f);
	CountdownText->SetTextRenderColor(FColor::Black);
	CountdownText->SetText(FText::FromString("Your mix will be ready in 2 minutes "));
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

void ACountdown::CountdownBegin()
{
	Time -= 1.0f;
	int32 Minutes = static_cast<int32>(Time) / 60;
	int32 Seconds = static_cast<int32>(Time) % 60;
	FormattedTime = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
	CountdownText->SetText(FText::FromString(FormattedTime));
	if (Time <= 0) {
		CountdownFinished();
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
	CountdownText->SetText(FText::FromString("Your mix will be ready in 2 minutes "));
}


void ACountdown::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);
	
	if (Character != nullptr) {
		
		float CheckCurrentVials = Character->CurrentVials;
		float MaxCurrentVials = Character->Max_Vials;
		bool CheckIfRespawned = Character->HasRespawned;

		if (CheckCurrentVials == MaxCurrentVials) {
			
			if (CheckIfRespawned == false) {
				if (bCollisionEnabled) {
					GetWorld()->GetTimerManager().SetTimer(CountdownTimer, this, &ACountdown::CountdownBegin, 1.0f, true);
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, TEXT("Button is clicked"));
					//SetActorEnableCollision(false); //want to deactivate trigger box
					UWorld* World = GetWorld();
					if (World) {
						UGameplayStatics::PlaySound2D(World, CountdownSound, 1.f, 1.f, 0.f);
					}
					bCollisionEnabled = false;
				}
			}
			
		
			else {
				SetActorEnableCollision(true);
				if (bCollisionEnabled == false) {
					GetWorld()->GetTimerManager().SetTimer(CountdownTimer, this, &ACountdown::CountdownBegin, 1.0f, true);
					SetActorEnableCollision(false); //want to deactivate trigger box
					UWorld* World = GetWorld();
					if (World) {
						UGameplayStatics::PlaySound2D(World, CountdownSound, 1.f, 1.f, 0.f);
					}
				}	
			}	
		}

		else {
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, TEXT("not allowed"));
		}

		
	}
}

void ACountdown::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//bCollisionEnabled = false;
	CountdownFinished();
}


