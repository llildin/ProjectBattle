// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/AnimPlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "InGamePlayer.h"

void UAnimPlayer::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<AInGamePlayer>(TryGetPawnOwner());

	if (Player)
	{
		Player->OnStateChanged.BindUObject(this, &UAnimPlayer::OnCurrentStateChanged);
	}
}

void UAnimPlayer::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Player)
	{
		Speed = Player->GetCharacterMovement()->Velocity.Size2D();
		Direction = UKismetAnimationLibrary::CalculateDirection(Player->GetCharacterMovement()->Velocity, Player->GetActorRotation());
	}
}


void UAnimPlayer::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (Speed == 0)
	{
		MoveState = EMoveState::Idle;
	}
	else if (Speed < 4000)
	{
		MoveState = EMoveState::Walk;
	}
}

void UAnimPlayer::OnCurrentStateChanged(ECurrentState NewState)
{
	CurrentState = NewState;
}
