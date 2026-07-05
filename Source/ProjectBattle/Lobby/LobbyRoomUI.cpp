// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyRoomUI.h"
#include "MyGameInstance.h"
#include "Components/VerticalBox.h"
#include "ObserberListUI.h"
#include "LobbyGameState.h"
#include "LobbyPlayerState.h"

void ULobbyRoomUI::NativeConstruct()
{
    Super::NativeConstruct();

    if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
    {
        GI->OnLobbyListChanged.AddUObject(this, &ULobbyRoomUI::RefreshUI);
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