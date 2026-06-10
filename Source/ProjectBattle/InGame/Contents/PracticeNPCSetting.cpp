// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/PracticeNPCSetting.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "InGamePlayer.h"

// Sets default values
APracticeNPCSetting::APracticeNPCSetting()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	NPCSetting = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NPCSetting"));
	NPCSetting->SetupAttachment(RootComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);

	SphereCollision->SetSphereRadius(300.0f);

	SphereCollision->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void APracticeNPCSetting::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APracticeNPCSetting::OnOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &APracticeNPCSetting::OnOverlapEnd);
}

// Called every frame
void APracticeNPCSetting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APracticeNPCSetting::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AInGamePlayer* PlayerChar = Cast<AInGamePlayer>(OtherActor);
    if (PlayerChar)
    {
		PlayerChar->bIsNPCSetting = true;
    }
}

void APracticeNPCSetting::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AInGamePlayer* PlayerChar = Cast<AInGamePlayer>(OtherActor);
	if (PlayerChar)
	{
		PlayerChar->bIsNPCSetting = false;
	}
}
