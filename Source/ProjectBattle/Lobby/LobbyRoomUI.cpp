// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyRoomUI.h"
#include "MyGameInstance.h"
#include "Components/VerticalBox.h"
#include "ObserberListUI.h"
#include "LobbyGameState.h"
#include "LobbyPlayerState.h"
#include "Components/Border.h"
#include "JoinGameUI.h"
#include "GamePlayerListUI.h"

void ULobbyRoomUI::NativeConstruct()
{
    Super::NativeConstruct();

    if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
    {
        GI->OnLobbyListChanged.AddUObject(this, &ULobbyRoomUI::RefreshUI);
    }

    Player01_JoinGameUIObject = CreateWidget<UJoinGameUI>(GetWorld(), Player01_JoinGameUIClass);
    Player01->SetContent(Player01_JoinGameUIObject);
    Player01_JoinGameUIObject->SetLobbyRoomUI(this, true);

    Player02_JoinGameUIObject = CreateWidget<UJoinGameUI>(GetWorld(), Player02_JoinGameUIClass);
    Player02->SetContent(Player02_JoinGameUIObject);
    Player02_JoinGameUIObject->SetLobbyRoomUI(this, false);

    Player01_GamePlayerListUIObject = CreateWidget<UGamePlayerListUI>(GetWorld(), Player01_GamePlayerListUIClass);
    Player01_GamePlayerListUIObject->SetLobbyRoomUI(this);

    Player02_GamePlayerListUIObject = CreateWidget<UGamePlayerListUI>(GetWorld(), Player02_GamePlayerListUIClass);
    Player02_GamePlayerListUIObject->SetLobbyRoomUI(this);

    LobbyGS = Cast<ALobbyGameState>(GetWorld()->GetGameState());
    if (LobbyGS)
    {
        LobbyGS->SetLobbyRoomUI(this);
    }

    RefreshUI();
}

void ULobbyRoomUI::RefreshUI()
{
    ObserverList->ClearChildren();
    ObserberListUIObject.Empty();

    AGameStateBase* GS = GetWorld()->GetGameState();
    if (!GS) return;

    for (APlayerState* PS : GS->PlayerArray)
    {
        ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(PS);
        if (!LobbyPS) continue;

        UObserberListUI* ChildWidget = CreateWidget<UObserberListUI>(GetWorld(), ObserberListUIClass);
        if (!ChildWidget) continue;

        ChildWidget->SetNickname(LobbyPS->Nickname);

        if (LobbyPS->PlayerRole == EPlayerRole::Spectator)
        {
            ObserverList->AddChildToVerticalBox(ChildWidget);
        }

        ObserberListUIObject.Add(ChildWidget);
    }
}

void ULobbyRoomUI::RefreshPlayer01UI()
{
    if (Player01->GetContent() != Player01_GamePlayerListUIObject)
    {
        Update_JoinPlayer(true);
    }
    Player01_GamePlayerListUIObject->RefreshUI(true);
}

void ULobbyRoomUI::RefreshPlayer02UI()
{
    if (Player02->GetContent() != Player02_GamePlayerListUIObject)
    {
        Update_JoinPlayer(false);
    }
    Player02_GamePlayerListUIObject->RefreshUI(false);
}

void ULobbyRoomUI::JoinPlayer(bool InIsPlayer01)
{
    C2S_JoinPlayer(InIsPlayer01);
}

bool ULobbyRoomUI::C2S_JoinPlayer_Validate(bool InIsPlayer01)
{
    return true;
}

void ULobbyRoomUI::C2S_JoinPlayer_Implementation(bool InIsPlayer01)
{
    LobbyGS->UpdateSlot(InIsPlayer01, GetOwningPlayerState(), EGameStatType::Join, 0);
}

void ULobbyRoomUI::Update_JoinPlayer(bool InIsPlayer01)
{
    if (InIsPlayer01)
    {
        Player01->SetContent(Player01_GamePlayerListUIObject);
    }
    else
    {
        Player02->SetContent(Player02_GamePlayerListUIObject);
    }
}
