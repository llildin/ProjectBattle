// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimyNotify_BasicAttackCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API UAnimyNotify_BasicAttackCheck : public UAnimNotify
{
	GENERATED_BODY()
	
	FORCEINLINE FString GetNotifyName_Implementation() const
	{
		return TEXT("AttackCheck_CPP");
	}

	UPROPERTY(EditAnywhere)
	float TraceRadius = 50.0f; 

	UPROPERTY(EditAnywhere)
	float Damage = 10.0f;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
