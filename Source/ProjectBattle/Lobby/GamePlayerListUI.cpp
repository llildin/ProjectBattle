// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/GamePlayerListUI.h"
#include "LobbyRoomUI.h"

void UGamePlayerListUI::SetLobbyRoomUI(ULobbyRoomUI* LobbyRoomUI)
{
	LobbyRoomObject = LobbyRoomUI;
}