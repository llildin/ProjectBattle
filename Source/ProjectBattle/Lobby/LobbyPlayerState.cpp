// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "MyGameInstance.h"
#include "LobbyGameState.h"
#include "LobbyRoomUI.h"

void ALobbyPlayerState::OnRep_Nickname()
{
    if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
    {
        GI->OnLobbyListChanged.Broadcast();
    }
}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, Nickname);
    DOREPLIFETIME(ALobbyPlayerState, PlayerRole);
}

void ALobbyPlayerState::CopyProperties(APlayerState* NewPlayerState)
{
}

void ALobbyPlayerState::BeginPlay()
{
    Super::BeginPlay();

    if (GetPlayerController() && GetPlayerController()->IsLocalController())
    {
        UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());

        if (GI)
        {
            C2S_SetNickname(GI->Nickname);
        }
    }
}

bool ALobbyPlayerState::C2S_SetNickname_Validate(const FString& InName)
{
    return true;
}

void ALobbyPlayerState::C2S_SetNickname_Implementation(const FString& InName)
{
    Nickname = InName;

    if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
    {
        GI->OnLobbyListChanged.Broadcast();
    }
}

void ALobbyPlayerState::OnRep_PlayerRole()
{
    if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
    {
        GI->OnLobbyListChanged.Broadcast();
    }
}

bool ALobbyPlayerState::C2S_JoinGame_Validate()
{
    return true;
}

void ALobbyPlayerState::C2S_JoinGame_Implementation()
{
    PlayerRole = EPlayerRole::Playing;

    if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
    {
        GI->OnLobbyListChanged.Broadcast();
    }
}

bool ALobbyPlayerState::C2S_JoinPlayer_Validate(bool InIsPlayer01)
{
    return true;
}

void ALobbyPlayerState::C2S_JoinPlayer_Implementation(bool InIsPlayer01)
{
    if (ALobbyGameState* GS = GetWorld()->GetGameState<ALobbyGameState>())
    {
        GS->UpdateSlot(InIsPlayer01, this, EGameStatType::Join, 0);
    }
}

bool ALobbyPlayerState::C2S_UpdatePlayer_Validate(bool InIsPlayer01, EGameStatType InGameStatType, float InUpDown)
{
    return true;
}

void ALobbyPlayerState::C2S_UpdatePlayer_Implementation(bool InIsPlayer01, EGameStatType InGameStatType, float InUpDown)
{
    if (ALobbyGameState* GS = GetWorld()->GetGameState<ALobbyGameState>())
    {
        GS->UpdateSlot(InIsPlayer01, this, InGameStatType, InUpDown);
    }
}

bool ALobbyPlayerState::C2S_CanclePlayer_Validate(bool InIsPlayer01)
{
    return true;
}

void ALobbyPlayerState::C2S_CanclePlayer_Implementation(bool InIsPlayer01)
{
    PlayerRole = EPlayerRole::Spectator;
    S2C_CanclePlayer(InIsPlayer01);
}

void ALobbyPlayerState::S2C_CanclePlayer_Implementation(bool InIsPlayer01)
{
    if (ALobbyGameState* GS = GetWorld()->GetGameState<ALobbyGameState>())
    {
        GS->LobbyRoomObject->Update_CanclePlayer(InIsPlayer01);
    }
}
