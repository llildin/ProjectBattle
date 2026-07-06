// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/InGamePlayerState.h"
#include "Net/UnrealNetwork.h"

void AInGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AInGamePlayerState, Nickname);
    DOREPLIFETIME(AInGamePlayerState, Stat_MaxHp);
    DOREPLIFETIME(AInGamePlayerState, Stat_MaxPosture);
    DOREPLIFETIME(AInGamePlayerState, Stat_AttackDamage);
    DOREPLIFETIME(AInGamePlayerState, Stat_PostureHealing);
    DOREPLIFETIME(AInGamePlayerState, IsPlayer);
}
