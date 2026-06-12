// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/AnimAttackPracticeNPC.h"
#include "AttackPracticeNPC.h"

void UAnimAttackPracticeNPC::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	NPC = Cast<AAttackPracticeNPC>(TryGetPawnOwner());

	if (NPC)
	{
		NPC->OnStateChanged.BindUObject(this, &UAnimAttackPracticeNPC::OnCurrentStateChanged);
	}
}

void UAnimAttackPracticeNPC::NativeUpdateAnimation(float DeltaSeconds)
{
}

void UAnimAttackPracticeNPC::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
}

void UAnimAttackPracticeNPC::OnCurrentStateChanged(ENPCState NewState)
{
	CurrentState = NewState;
}
