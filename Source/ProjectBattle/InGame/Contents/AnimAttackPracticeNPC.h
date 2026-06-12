// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "AttackPracticeNPC.h"

#include "AnimAttackPracticeNPC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API UAnimAttackPracticeNPC : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ENPCState CurrentState = ENPCState::Idle;

	void OnCurrentStateChanged(ENPCState NewState);

	AAttackPracticeNPC* NPC;


};
