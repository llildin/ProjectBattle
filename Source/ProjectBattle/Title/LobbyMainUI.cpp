// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/LobbyMainUI.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "LobbyListUI.h"

void ULobbyMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Create)
	{
		Btn_Create->OnClicked.RemoveDynamic(this, &ULobbyMainUI::OnClickedCreateButton);
		Btn_Create->OnClicked.AddDynamic(this, &ULobbyMainUI::OnClickedCreateButton);
	}

	if (Btn_Refresh)
	{
		Btn_Refresh->OnClicked.RemoveDynamic(this, &ULobbyMainUI::OnClickedRefreshButton);
		Btn_Refresh->OnClicked.AddDynamic(this, &ULobbyMainUI::OnClickedRefreshButton);
	}

	if (Btn_Quit)
	{
		Btn_Quit->OnClicked.RemoveDynamic(this, &ULobbyMainUI::OnClickedQuitButton);
		Btn_Quit->OnClicked.AddDynamic(this, &ULobbyMainUI::OnClickedQuitButton);
	}

	SetInfo();
}

void ULobbyMainUI::SetInfo()
{
	LobbyListUIObject.Empty();

	for (int i = 0; i < 10; i++)
	{
		if (LobbyListUIClass == nullptr)
		{
			continue;
		}

		ULobbyListUI* ChildWidget = CreateWidget<ULobbyListUI>(GetWorld(), LobbyListUIClass);

		if (ChildWidget == nullptr)
		{
			continue;
		}

		RoomList->AddChildToVerticalBox(ChildWidget);

		LobbyListUIObject.Add(ChildWidget);
	}

	RefreshUI();
}

void ULobbyMainUI::RefreshUI()
{
	const int32 SessionLength = SessionInfos.Num();

	for (int32 i = 0; i < LobbyListUIObject.Num(); i++)
	{
		const int32 index = i;

		if (index < SessionLength)
		{
			LobbyListUIObject[index]->SetVisibility(ESlateVisibility::Visible);
			FBlueprintSessionResult Result = SessionInfos[index];
			LobbyListUIObject[index]->SetInfo(Result);
		}
		else
		{
			LobbyListUIObject[index]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void ULobbyMainUI::OnClickedCreateButton()
{
	OpenCreateRoomUI();
}

void ULobbyMainUI::OnClickedRefreshButton()
{
	SetSessionInfos();
}

void ULobbyMainUI::OnClickedQuitButton()
{
	OnQuitLobby.ExecuteIfBound();
}

