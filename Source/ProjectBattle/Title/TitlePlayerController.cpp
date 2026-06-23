// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/TitlePlayerController.h"
#include "TitleMainUI.h"
#include "LobbyMainUI.h"

void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		TitleMainUIObject = CreateWidget<UTitleMainUI>(this, TitleMainUIClass);
		TitleMainUIObject->AddToViewport();
		TitleMainUIObject->SetVisibility(ESlateVisibility::Visible);

		LobbyMainUIObject = CreateWidget<ULobbyMainUI>(this, LobbyMainUIClass);
		LobbyMainUIObject->AddToViewport();
		LobbyMainUIObject->SetVisibility(ESlateVisibility::Collapsed);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());

		TitleMainUIObject->OnStartGame.BindLambda([this]()
			{
				TitleMainUIObject->SetVisibility(ESlateVisibility::Collapsed);
				LobbyMainUIObject->SetVisibility(ESlateVisibility::Visible);
			});
	}
}
