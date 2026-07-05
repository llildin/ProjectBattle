// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/GamePlayerListUI.h"
#include "LobbyRoomUI.h"
#include "LobbyGameState.h"
#include "Components/TextBlock.h"

void UGamePlayerListUI::SetLobbyRoomUI(ULobbyRoomUI* LobbyRoomUI)
{
	LobbyRoomObject = LobbyRoomUI;

    LobbyGS = Cast<ALobbyGameState>(GetWorld()->GetGameState());
}

void UGamePlayerListUI::RefreshUI(bool InIsPlayer01)
{
	if (InIsPlayer01)
	{
		Txt_Nickname->SetText(FText::FromString(LobbyGS->PlayerSlot1.Nickname));
	}
	else
	{
		Txt_Nickname->SetText(FText::FromString(LobbyGS->PlayerSlot2.Nickname));
	}
}