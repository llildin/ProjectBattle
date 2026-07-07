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
#include "Components/Button.h"

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

    if (Btn_GameStart)
    {
        Btn_GameStart->OnClicked.RemoveDynamic(this, &ULobbyRoomUI::OnClickedGameStartButton);
        Btn_GameStart->OnClicked.AddDynamic(this, &ULobbyRoomUI::OnClickedGameStartButton);
    }

    if (GetOwningPlayer()->HasAuthority())
    {
        Btn_GameStart->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        Btn_GameStart->SetVisibility(ESlateVisibility::Collapsed);
    }

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
    if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetOwningPlayerState()))
    {
        LobbyPS->C2S_JoinPlayer(InIsPlayer01);
    }
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

void ULobbyRoomUI::CanclePlayer(bool InIsPlayer01)
{
    if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetOwningPlayerState()))
    {
        LobbyPS->C2S_CanclePlayer(InIsPlayer01);
    }
}

void ULobbyRoomUI::Update_CanclePlayer(bool InIsPlayer01)
{
    if (InIsPlayer01)
    {
        Player01->SetContent(Player01_JoinGameUIObject);
    }
    else
    {
        Player02->SetContent(Player02_JoinGameUIObject);
    }

    RefreshUI();
}

void ULobbyRoomUI::OnClickedGameStartButton()
{
    if (LobbyGS->PlayerSlot1.bIsReady && LobbyGS->PlayerSlot2.bIsReady)
    {
        if (ALobbyPlayerState* LobbyPS1 = Cast<ALobbyPlayerState>(LobbyGS->PlayerSlot1.OwningPlayerState))
        {
            LobbyPS1->Nickname = LobbyGS->PlayerSlot1.Nickname;
            LobbyPS1->Stat_MaxHp = LobbyGS->PlayerSlot1.MaxHp;
            LobbyPS1->Stat_MaxPosture = LobbyGS->PlayerSlot1.MaxPosture;
            LobbyPS1->Stat_PostureHealing = LobbyGS->PlayerSlot1.PostureHealing;
            LobbyPS1->Stat_AttackDamage = LobbyGS->PlayerSlot1.AttackDamage;
        }
        
        if (ALobbyPlayerState* LobbyPS2 = Cast<ALobbyPlayerState>(LobbyGS->PlayerSlot2.OwningPlayerState))
        {
            LobbyPS2->Nickname = LobbyGS->PlayerSlot2.Nickname;
            LobbyPS2->Stat_MaxHp = LobbyGS->PlayerSlot2.MaxHp;
            LobbyPS2->Stat_MaxPosture = LobbyGS->PlayerSlot2.MaxPosture;
            LobbyPS2->Stat_PostureHealing = LobbyGS->PlayerSlot2.PostureHealing;
            LobbyPS2->Stat_AttackDamage = LobbyGS->PlayerSlot2.AttackDamage;
        }
        GetWorld()->ServerTravel(TEXT("/Game/Map/GameRoom"));
    }
}
