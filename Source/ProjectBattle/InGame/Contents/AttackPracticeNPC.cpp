// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/AttackPracticeNPC.h"
#include "InGame/AttackFunction.h"

void AAttackPracticeNPC::BeginPlay()
{
	Super::BeginPlay();
	
	SetAnimRootMotionTranslationScale(0.0f);
}

void AAttackPracticeNPC::SetState(ENPCState State)
{
	if (CurrentState == State)
	{
		return;
	}

	CurrentState = State;
	OnStateChanged.ExecuteIfBound(State);

	if (State == ENPCState::BasicAttack)
	{
		bIsNPCAttacking = true;
		StartNPCBasicComboAttack();
	}
	else
	{
		bIsNPCAttacking = false;
		NPCComboCount = 0;
	}
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
	if (bIsNPCAttacking)
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
