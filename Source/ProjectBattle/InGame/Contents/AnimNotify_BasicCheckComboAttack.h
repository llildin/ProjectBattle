// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_BasicCheckComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API UAnimNotify_BasicCheckComboAttack : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	FORCEINLINE FString GetNotifyName_Implementation() const
	{
		return TEXT("CheckCombo_CPP");
	}

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
