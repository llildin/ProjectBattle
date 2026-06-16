// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/AnimNotify_FinishOnDamage.h"
#include "Human.h"

void UAnimNotify_FinishOnDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AHuman* Human = Cast<AHuman>(MeshComp->GetOwner());
	if (Human)
	{
		if (Human->PrevState == ECurrentState::No_Battle)
		{
			Human->SetCurrentState(Human->PrevState);
		}
		else
		{
			Human->SetCurrentState(ECurrentState::Battle);
		}
		UE_LOG(LogTemp, Warning, TEXT("ddddd"));
	}
}
