// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, Nickname);
}

void ALobbyPlayerState::CopyProperties(APlayerState* NewPlayerState)
{
}
