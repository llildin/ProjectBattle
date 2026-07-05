// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyGameState.h"
#include "Net/UnrealNetwork.h"
#include "MyGameInstance.h"

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyGameState, PlayerCount);
	DOREPLIFETIME(ALobbyGameState, PlayerSlot1);
	DOREPLIFETIME(ALobbyGameState, PlayerSlot2);
}

void ALobbyGameState::OnRep_PlayerCount()
{
	if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
	{
		GI->OnLobbyListChanged.Broadcast();
	}
}

void ALobbyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	PlayerCount = PlayerArray.Num();
	OnRep_PlayerCount();
}

void ALobbyGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	PlayerCount = PlayerArray.Num();
	OnRep_PlayerCount();
}

void ALobbyGameState::OnRep_PlayerSlot1()
{
}

void ALobbyGameState::OnRep_PlayerSlot2()
{
}
