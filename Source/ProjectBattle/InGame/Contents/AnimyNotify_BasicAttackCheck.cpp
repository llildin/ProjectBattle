// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/AnimyNotify_BasicAttackCheck.h"
#include "InGamePlayer.h"

void UAnimyNotify_BasicAttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AInGamePlayer* Pawn = Cast<AInGamePlayer>(MeshComp->GetOwner());
	if (Pawn)
	{
		Pawn->BasicAttackTrace();
	}
}
