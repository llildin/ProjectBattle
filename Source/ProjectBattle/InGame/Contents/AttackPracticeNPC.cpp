// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/AttackPracticeNPC.h"

void AAttackPracticeNPC::BeginPlay()
{
	Super::BeginPlay();


}

void AAttackPracticeNPC::SetState(ENPCState State)
{
	CurrentState = State;
	OnStateChanged.ExecuteIfBound(State);
}