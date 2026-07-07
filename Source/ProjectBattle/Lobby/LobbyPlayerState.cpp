// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "MyGameInstance.h"
#include "LobbyGameState.h"
#include "LobbyRoomUI.h"
#include "InGame/Contents/InGamePlayerState.h"

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

    DOREPLIFETIME(ALobbyPlayerState, Stat_MaxHp);
    DOREPLIFETIME(ALobbyPlayerState, Stat_MaxPosture);
    DOREPLIFETIME(ALobbyPlayerState, Stat_AttackDamage);
    DOREPLIFETIME(ALobbyPlayerState, Stat_PostureHealing);
    DOREPLIFETIME(ALobbyPlayerState, IsPlayer);
}

void ALobbyPlayerState::CopyProperties(APlayerState* NewPlayerState)
{
    Super::CopyProperties(NewPlayerState);

    if (AInGamePlayerState* NewPS = Cast<AInGamePlayerState>(NewPlayerState))
    {
        NewPS->Nickname = Nickname;
        NewPS->Stat_MaxHp = Stat_MaxHp;
        NewPS->Stat_MaxPosture = Stat_MaxPosture;
        NewPS->Stat_AttackDamage = Stat_AttackDamage;
        NewPS->Stat_PostureHealing = Stat_PostureHealing;

        if (PlayerRole == EPlayerRole::Playing)
        {
            NewPS->IsPlayer = true;
        }
        else
        {
            NewPS->IsPlayer = false;
        }
    }
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
