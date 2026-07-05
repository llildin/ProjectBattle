// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"
#include "LobbyRoomUI.h"

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		LobbyRoomUIObject = CreateWidget<ULobbyRoomUI>(this, LobbyRoomUIClass);
		LobbyRoomUIObject->AddToViewport();
		LobbyRoomUIObject->SetVisibility(ESlateVisibility::Visible);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}
