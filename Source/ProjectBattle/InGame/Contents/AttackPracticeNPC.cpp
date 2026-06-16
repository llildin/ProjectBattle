// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/AttackPracticeNPC.h"
#include "InGame/AttackFunction.h"

void AAttackPracticeNPC::BeginPlay()
{
	Super::BeginPlay();
	
	SetAnimRootMotionTranslationScale(0.0f);

	CurrentState = ECurrentState::Attack;
	PrevState = ECurrentState::No_Battle;
}

void AAttackPracticeNPC::SetCurrentState(ECurrentState NewState)
{
	Super::SetCurrentState(NewState);

	if (CurrentState == NewState)
	{
		return;
	}
	
	CurrentState = NewState;
	OnStateChanged.ExecuteIfBound(NewState);

	if (NewState == ECurrentState::BasicAttack)
	{
		bIsAttacking = true;
		StartNPCBasicComboAttack();
	}
	else
	{
		bIsAttacking = false;
		NPCComboCount = 0;
	}
}

void AAttackPracticeNPC::RefreshAttackSetting()
{
	Super::RefreshAttackSetting();
}

void AAttackPracticeNPC::StartNPCBasicComboAttack()
{
	NPCComboCount = 1;

	PlayNPCBasicComboAttackMontage();
}

void AAttackPracticeNPC::NPCCheckBasicComboAttack()
{
	if (NPCComboCount < 4)
	{
		NPCComboCount++;
		PlayNPCBasicComboAttackMontage();
	}
}

void AAttackPracticeNPC::PlayNPCBasicComboAttackMontage()
{
	if (bIsAttacking)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AttackSectionName = FString::Printf(TEXT("BasicAttack0%d"), NPCComboCount);

			float MontageLength = PlayAnimMontage(AttackMontage, 1.0f, FName(AttackSectionName));
			if (MontageLength > 0)
			{
				FOnMontageEnded EndDelegate;
				EndDelegate.BindLambda([WeakThis = TWeakObjectPtr<AAttackPracticeNPC>(this)](UAnimMontage* Montage, bool bInterrupted) {
					if (!bInterrupted && WeakThis.IsValid())
					{
						WeakThis->NPCComboCount = 0;
						WeakThis->StartNPCBasicComboAttack();
					}
					});
				AnimInstance->Montage_SetEndDelegate(EndDelegate);
			}
		}
	}
}

void AAttackPracticeNPC::NPCBasicAttackTrace()
{
	UAttackFunction::BasicAttackTraceShot(DT_AttackData, AttackSectionName, this);
}
