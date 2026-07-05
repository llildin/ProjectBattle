// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyGameState.h"
#include "Net/UnrealNetwork.h"
#include "MyGameInstance.h"
#include "LobbyRoomUI.h"
#include "LobbyPlayerState.h"

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyGameState, PlayerCount);
	DOREPLIFETIME(ALobbyGameState, PlayerSlot1);
	DOREPLIFETIME(ALobbyGameState, PlayerSlot2);
}

void ALobbyGameState::OnRep_PlayerCount()
{
	if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
	{
		GI->OnLobbyListChanged.Broadcast();
	}
}

void ALobbyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	PlayerCount = PlayerArray.Num();
	OnRep_PlayerCount();
}

void ALobbyGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
	PlayerCount = PlayerArray.Num();
	OnRep_PlayerCount();
}

void ALobbyGameState::OnRep_PlayerSlot1()
{
	LobbyRoomObject->RefreshPlayer01UI();
}

void ALobbyGameState::OnRep_PlayerSlot2()
{
	LobbyRoomObject->RefreshPlayer02UI();
}

void ALobbyGameState::SetLobbyRoomUI(ULobbyRoomUI* LobbyRoomUI)
{
	LobbyRoomObject = LobbyRoomUI;
}

void ALobbyGameState::UpdateSlot(bool IsPlayer01, APlayerState* PlayerState, EGameStatType GameStatType, float UpDown)
{
	if (!HasAuthority())
	{
		return;
	}

	FGameSlotData* Slot;

	if (IsPlayer01)
	{
		Slot = &PlayerSlot1;
	}
	else
	{
		Slot = &PlayerSlot2;
	}

	ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(PlayerState);

	if (Slot->OwningPlayerState == LobbyPS)
	{
		if (Slot->RemainingPT <= 0)
		{
			return;
		}

		switch (GameStatType)
		{
		case EGameStatType::MaxHp:
		{
			if (Slot->MaxHp <= 0 || Slot->MaxHp >= 10)
			{
				return;
			}
			Slot->MaxHp += UpDown;
			break;
		}
		case EGameStatType::MaxPosture:
		{
			if (Slot->MaxPosture <= 0 || Slot->MaxPosture >= 10)
			{
				return;
			}
			Slot->MaxPosture += UpDown;
			break;
		}
		case EGameStatType::AttackDamage:
		{
			if (Slot->AttackDamage <= 0 || Slot->AttackDamage >= 10)
			{
				return;
			}
			Slot->AttackDamage += UpDown;
			break;
		}
		case EGameStatType::PostureHealing:
		{
			if (Slot->PostureHealing <= 0 || Slot->PostureHealing >= 10)
			{
				return;
			}
			Slot->PostureHealing += UpDown;
			break;
		}
		}
		Slot->RemainingPT += UpDown;
	}
	else
	{
		Slot->OwningPlayerState = LobbyPS;
		Slot->Nickname = LobbyPS->Nickname;
		Slot->MaxHp = 3.0f;
		Slot->MaxPosture = 3.0f;
		Slot->AttackDamage = 3.0f;
		Slot->PostureHealing = 3.0f;
		Slot->RemainingPT = 10;
		Slot->bIsReady = false;
	}

	OnRep_PlayerSlot1();
	OnRep_PlayerSlot2();
}
