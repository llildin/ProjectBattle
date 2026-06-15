// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/AnimyNotify_BasicAttackCheck.h"
#include "InGamePlayer.h"
#include "AttackPracticeNPC.h"

void UAnimyNotify_BasicAttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AInGamePlayer* Player = Cast<AInGamePlayer>(MeshComp->GetOwner());
	if (Player)
	{
		Player->BasicAttackTrace();
	}

	AAttackPracticeNPC* NPC = Cast<AAttackPracticeNPC>(MeshComp->GetOwner());
	if (NPC)
	{
		NPC->NPCBasicAttackTrace();
	}
}
