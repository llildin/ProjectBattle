// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/LobbyListUI.h"
#include "Components/TextBlock.h"

void ULobbyListUI::SetInfo(FBlueprintSessionResult InSessionResult)
{
	Result = InSessionResult;

	RefreshUI();
}

void ULobbyListUI::RefreshUI()
{
	//방이름
	FString SessionName;
	Result.OnlineResult.Session.SessionSettings.Get("SESSION_NAME", OUT SessionName);
	Txt_RoomName->SetText(FText::FromString(SessionName));

	//인원수
	const int32 currentPlayers = Result.OnlineResult.Session.SessionSettings.NumPublicConnections -
		Result.OnlineResult.Session.NumOpenPublicConnections;
	const int32 MaxPlayers = Result.OnlineResult.Session.SessionSettings.NumPublicConnections;
	FString PlayerCountString = FString::Printf(TEXT("(%d / %d)"), currentPlayers, MaxPlayers);

	Txt_PlayerCount->SetText(FText::FromString(PlayerCountString));
}