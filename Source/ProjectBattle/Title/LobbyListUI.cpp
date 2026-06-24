// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/LobbyListUI.h"

void ULobbyListUI::SetInfo(FBlueprintSessionResult InSessionResult)
{
	Result = InSessionResult;

	RefreshUI();
}

void ULobbyListUI::RefreshUI()
{
}