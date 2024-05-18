 // Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger.h"
#include "PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "cameraSwitchInterface.h"
#include "Kismet/GameplayStatics.h"
 // Sets default values
ATrigger::ATrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Root component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	//Create a collider for trigger
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionTrigger"));
	BoxCollider->SetBoxExtent(FVector(32.0f, 32.0f, 32.0f));
	BoxCollider->SetCollisionProfileName("Trigger");
	BoxCollider->SetGenerateOverlapEvents(true);
	BoxCollider->SetupAttachment(SceneComponent);

	//Static Mesh
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(BoxCollider);

	SetRootComponent(SceneComponent);
}

// Called when the game starts or when spawned
void ATrigger::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ATrigger::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATrigger::OnOverlapEnd);
}


void ATrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (this->Tags.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, (TEXT("NO TAGS ON TRIGGER")));
		return;
	}
	if (OtherActor->GetClass()->ImplementsInterface(UcameraSwitchInterface::StaticClass()) && OtherActor != this)
	{
		Cast<IcameraSwitchInterface>(OtherActor)->RunOnTagOverlap(this->Tags[0].ToString());
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("Begin Overlapp"));

	}
	
}

void ATrigger::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if (this->Tags.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, (TEXT("NO TAGS ON TRIGGER")));
		return;
	}
	if (OtherActor->GetClass()->ImplementsInterface(UcameraSwitchInterface::StaticClass()) && OtherActor != this )
	{
		Cast<IcameraSwitchInterface>(OtherActor)->RunOnTagEndOverlap(this->Tags[0].ToString());
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("End Overlapp"));

	}
}

