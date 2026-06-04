// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/AnimNotify_BasicCheckComboAttack.h"
#include "InGamePlayer.h"

void UAnimNotify_BasicCheckComboAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AInGamePlayer* Pawn = Cast<AInGamePlayer>(MeshComp->GetOwner());
	if (Pawn)
	{
		Pawn->BasicCheckComboAttack();
	}
}
