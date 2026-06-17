// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/Human.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/GameStateBase.h"

// Sets default values
AHuman::AHuman()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Katana = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Katana"));
	Katana->SetupAttachment(GetMesh(), TEXT("Katana_Socket"));

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
		FRotator(0, -90.f, 0)
	);
}

// Called when the game starts or when spawned
void AHuman::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHuman::SetCurrentState(ECurrentState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}
}

void AHuman::RefreshAttackSetting()
{
}

float AHuman::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!CheckIsDamaged())
	{
		return 0.0f;
	}

	if (CurrentState == ECurrentState::Guard)
	{
		CheckGuard();

		return 0.0f;
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimInstance->IsAnyMontagePlaying())
	{
		RefreshAttackSetting();
		AnimInstance->Montage_Stop(0.1f);
	}

	PrevState = CurrentState;
	SetCurrentState(ECurrentState::On_Damaged);

	if (ActualDamage <= 0.0f) return 0.0f;

	HP = FMath::Clamp(HP - ActualDamage, 0.0f, MaxHP);

	if (HP <= 0.0f)
	{
		// Die();
	}

	return ActualDamage;
}

bool AHuman::CheckIsDamaged()
{
	if (CurrentState == ECurrentState::Interact || CurrentState == ECurrentState::Rolling ||
		CurrentState == ECurrentState::On_Damaged)
	{
		return false;
	}

	return true;
}

void AHuman::CheckGuard()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		float CurrentTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		float MontageLength = 0.0f;

		if ((CurrentTime - GuardStartTime) <= 0.15f)
		{
			MontageLength = PlayAnimMontage(Guard_Perfect_Hit_Montage);
		}
		else
		{
			MontageLength = PlayAnimMontage(Guard_Hit_Montage);
		}

		if (MontageLength > 0)
		{

			FOnMontageEnded EndDelegate;

			EndDelegate.BindLambda([WeakThis = TWeakObjectPtr<AHuman>(this)](UAnimMontage* Montage, bool bInterrupted) {
				if (!bInterrupted && WeakThis.IsValid())
				{
					WeakThis->SetCurrentState(ECurrentState::Guard);
				}
				});
			AnimInstance->Montage_SetEndDelegate(EndDelegate);
		}
	}
}
