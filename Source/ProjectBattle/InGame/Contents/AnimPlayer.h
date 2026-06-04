// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "InGamePlayer.h"

#include "AnimPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API UAnimPlayer : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EMoveState MoveState = EMoveState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ECurrentState CurrentState = ECurrentState::No_Battle;

	void OnCurrentStateChanged(ECurrentState NewState);
	
	AInGamePlayer* Player;
};
