// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/Human.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/GameStateBase.h"
#include "InGamePlayer.h"
#include "Net/UnrealNetwork.h"

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

	if (HitTime > 0.0f)
	{
		HitTime -= DeltaTime;
		return;
	}

	if (HP != 0.0f && Posture != 0.0f)
	{
		PostureHeal = POSTUREHEAL * DeltaTime;
		HpRate = HP / MaxHP;

		if (HpRate >= 0.75f)
		{
			Posture = FMath::Clamp(Posture - PostureHeal, 0.0f, MaxPosture);
		}
		else if (HpRate >= 0.5 && HpRate < 0.75)
		{
			Posture = FMath::Clamp(Posture - (PostureHeal * 0.66f), 0.0f, MaxPosture);
		}
		else if (HpRate >= 0.25 && HpRate < 0.5)
		{
			Posture = FMath::Clamp(Posture - (PostureHeal * 0.33f), 0.0f, MaxPosture);
		}
		else
		{
			Posture = FMath::Clamp(Posture - (PostureHeal * 0.01f), 0.0f, MaxPosture);
		}
	}
}

void AHuman::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHuman, CurrentState);
	DOREPLIFETIME(AHuman, HP);
}

void AHuman::OnRep_CurrentState()
{
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

// 서버에서만 실행
float AHuman::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!CheckIsDamaged())
	{
		return 0.0f;
	}

	if (CurrentState == ECurrentState::Guard)
	{
		CheckGuard(ActualDamage, DamageCauser);

		return 0.0f;
	}

	S2C_TakeDamage(ActualDamage);

	// CurrentState는 Replicated 라 알려주지 않아도 됨
	SetCurrentState(ECurrentState::On_Damaged);

	// HP는 Replicated라 알려주지 않아도 됨
	HP = FMath::Clamp(HP - ActualDamage, 0.0f, MaxHP);

	return ActualDamage;
}

void AHuman::S2C_TakeDamage_Implementation(float ActualDamage)
{
	// HitTime -> 클라알려줘야함
	HitTime = HIT_TIME;

	// S2C로 해줘야함
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimInstance->IsAnyMontagePlaying())
	{
		RefreshAttackSetting();
		AnimInstance->Montage_Stop(0.1f);
	}

	// PrevState -> 클라알려줘야함
	PrevState = CurrentState;

	// Posture는 클라알려줘야함
	Posture = FMath::Clamp(Posture + (ActualDamage * NormalPostureDamageRate), 0.0f, MaxPosture);
}

// 서버에서만 실행
bool AHuman::CheckIsDamaged()
{
	if (CurrentState == ECurrentState::Interact || CurrentState == ECurrentState::Rolling ||
		CurrentState == ECurrentState::On_Damaged)
	{
		return false;
	}

	return true;
}

void AHuman::CheckGuard(float Damage, AActor* Attacker)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		float CurrentTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		float MontageLength = 0.0f;

		if ((CurrentTime - GuardStartTime) <= 0.15f)
		{
			MontageLength = Guard_Perfect_Hit_Montage->GetPlayLength();
		}
		else
		{
			MontageLength = Guard_Hit_Montage->GetPlayLength();
		}

		S2C_CheckGuard(CurrentTime, MontageLength, Damage, Attacker);

		// 이 if문은 서버에서만 실행
		if (MontageLength > 0)
		{

			FOnMontageEnded EndDelegate;

			EndDelegate.BindLambda([WeakThis = TWeakObjectPtr<AHuman>(this)](UAnimMontage* Montage, bool bInterrupted) {
				if (!bInterrupted && WeakThis.IsValid() && WeakThis->CurrentState != ECurrentState::Battle)
				{
					WeakThis->SetCurrentState(ECurrentState::Guard);
				}
				});
			AnimInstance->Montage_SetEndDelegate(EndDelegate);
		}
	}
}

void AHuman::S2C_CheckGuard_Implementation(float CurrentTime, float MontageLength, float Damage, AActor* Attacker)
{
	//이 if문 내부 전부 S2C로 구현
	HitTime = HIT_TIME;

	if ((CurrentTime - GuardStartTime) <= 0.15f)
	{
		MontageLength = PlayAnimMontage(Guard_Perfect_Hit_Montage);
		Posture = FMath::Clamp(Posture + (Damage * PerfectGuardPostureDamageRate), 0.0f, MaxPosture);

		AHuman* AttackPlayer = Cast<AHuman>(Attacker);
		AttackPlayer->Posture = FMath::Clamp(AttackPlayer->Posture + (Damage * GuardPostureDamageRate), 0.0f, AttackPlayer->MaxPosture);
		AttackPlayer->HitTime = HIT_TIME;

		//if문으로 AttackPlayer가 본인일 경우만 실행
		if (AInGamePlayer* InGamePlayer = Cast<AInGamePlayer>(AttackPlayer))
		{
			if (InGamePlayer->IsLocallyControlled())
			{
				InGamePlayer->UpDateUI();
			}
		}
	}
	else
	{
		MontageLength = PlayAnimMontage(Guard_Hit_Montage);
		Posture = FMath::Clamp(Posture + (Damage * GuardPostureDamageRate), 0.0f, MaxPosture);
	}
}
