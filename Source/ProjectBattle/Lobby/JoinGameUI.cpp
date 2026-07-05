// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/JoinGameUI.h"
#include "LobbyRoomUI.h"
#include "Components/Button.h"
#include "LobbyGameState.h"

void UJoinGameUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_JoinGame)
	{
		Btn_JoinGame->OnClicked.RemoveDynamic(this, &UJoinGameUI::OnClickedJoinGameButton);
		Btn_JoinGame->OnClicked.AddDynamic(this, &UJoinGameUI::OnClickedJoinGameButton);
	}
}

void UJoinGameUI::SetLobbyRoomUI(ULobbyRoomUI* LobbyRoomUI, bool InIsPlayer01)
{
	LobbyRoomObject = LobbyRoomUI;

	IsPlayer01 = InIsPlayer01;
}

void UJoinGameUI::OnClickedJoinGameButton()
{
	LobbyRoomObject->JoinPlayer(IsPlayer01);
}
