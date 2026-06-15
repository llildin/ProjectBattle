// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/AnimNotify_BasicCheckComboAttack.h"
#include "InGamePlayer.h"
#include "AttackPracticeNPC.h"

void UAnimNotify_BasicCheckComboAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AInGamePlayer* Player = Cast<AInGamePlayer>(MeshComp->GetOwner());
	if (Player)
	{
		Player->BasicCheckComboAttack();
	}

	AAttackPracticeNPC* NPC = Cast<AAttackPracticeNPC>(MeshComp->GetOwner());
	if (NPC)
	{
		NPC->NPCCheckBasicComboAttack();
	}
}
