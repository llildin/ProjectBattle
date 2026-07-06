// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/GamePlayerListUI.h"
#include "LobbyRoomUI.h"
#include "LobbyGameState.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "LobbyPlayerState.h"

void UGamePlayerListUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Cancle)
	{
		Btn_Cancle->OnClicked.RemoveDynamic(this, &UGamePlayerListUI::OnClickedCancleGameButton);
		Btn_Cancle->OnClicked.AddDynamic(this, &UGamePlayerListUI::OnClickedCancleGameButton);
	}

	if (Btn_MaxHpDown)
	{
		Btn_MaxHpDown->OnClicked.RemoveDynamic(this, &UGamePlayerListUI::OnClickedMaxHpDownGameButton);
		Btn_MaxHpDown->OnClicked.AddDynamic(this, &UGamePlayerListUI::OnClickedMaxHpDownGameButton);
	}

	if (Btn_MaxHpUp)
	{
		Btn_MaxHpUp->OnClicked.RemoveDynamic(this, &UGamePlayerListUI::OnClickedMaxHpUpGameButton);
		Btn_MaxHpUp->OnClicked.AddDynamic(this, &UGamePlayerListUI::OnClickedMaxHpUpGameButton);
	}

	if (Btn_MaxPostureDown)
	{
		Btn_MaxPostureDown->OnClicked.RemoveDynamic(this, &UGamePlayerListUI::OnClickedMaxPostureDownGameButton);
		Btn_MaxPostureDown->OnClicked.AddDynamic(this, &UGamePlayerListUI::OnClickedMaxPostureDownGameButton);
	}

	if (Btn_MaxPostureUp)
	{
		Btn_MaxPostureUp->OnClicked.RemoveDynamic(this, &UGamePlayerListUI::OnClickedMaxPostureUpGameButton);
		Btn_MaxPostureUp->OnClicked.AddDynamic(this, &UGamePlayerListUI::OnClickedMaxPostureUpGameButton);
	}

	if (Btn_AttackDamageDown)
	{
		Btn_AttackDamageDown->OnClicked.RemoveDynamic(this, &UGamePlayerListUI::OnClickedAttackDamageDownGameButton);
		Btn_AttackDamageDown->OnClicked.AddDynamic(this, &UGamePlayerListUI::OnClickedAttackDamageDownGameButton);
	}

	if (Btn_AttackDamageUp)
	{
		Btn_AttackDamageUp->OnClicked.RemoveDynamic(this, &UGamePlayerListUI::OnClickedAttackDamageUpGameButton);
		Btn_AttackDamageUp->OnClicked.AddDynamic(this, &UGamePlayerListUI::OnClickedAttackDamageUpGameButton);
	}

	if (Btn_PostureHealingDown)
	{
		Btn_PostureHealingDown->OnClicked.RemoveDynamic(this, &UGamePlayerListUI::OnClickedPostureHealingDownGameButton);
		Btn_PostureHealingDown->OnClicked.AddDynamic(this, &UGamePlayerListUI::OnClickedPostureHealingDownGameButton);
	}

	if (Btn_PostureHealingUp)
	{
		Btn_PostureHealingUp->OnClicked.RemoveDynamic(this, &UGamePlayerListUI::OnClickedPostureHealingUpGameButton);
		Btn_PostureHealingUp->OnClicked.AddDynamic(this, &UGamePlayerListUI::OnClickedPostureHealingUpGameButton);
	}

	if (Btn_Ready)
	{
		Btn_Ready->OnClicked.RemoveDynamic(this, &UGamePlayerListUI::OnClickedReadyGameButton);
		Btn_Ready->OnClicked.AddDynamic(this, &UGamePlayerListUI::OnClickedReadyGameButton);
	}
}

void UGamePlayerListUI::SetLobbyRoomUI(ULobbyRoomUI* LobbyRoomUI)
{
	LobbyRoomObject = LobbyRoomUI;

    LobbyGS = Cast<ALobbyGameState>(GetWorld()->GetGameState());
}

void UGamePlayerListUI::RefreshUI(bool InIsPlayer01)
{
	if (InIsPlayer01)
	{
		Txt_Nickname->SetText(FText::FromString(LobbyGS->PlayerSlot1.Nickname));
		IsPlayer01 = InIsPlayer01;

		Txt_MaxHp->SetText(FText::AsNumber(FMath::RoundToInt(LobbyGS->PlayerSlot1.MaxHp)));
		Txt_MaxPosture->SetText(FText::AsNumber(FMath::RoundToInt(LobbyGS->PlayerSlot1.MaxPosture)));
		Txt_AttackDamage->SetText(FText::AsNumber(FMath::RoundToInt(LobbyGS->PlayerSlot1.AttackDamage)));
		Txt_PostureHealing->SetText(FText::AsNumber(FMath::RoundToInt(LobbyGS->PlayerSlot1.PostureHealing)));
		Txt_RemaingPt->SetText(FText::AsNumber(LobbyGS->PlayerSlot1.RemainingPT));

		Bar_MaxHp->SetPercent(LobbyGS->PlayerSlot1.MaxHp / 10);
		Bar_MaxPosture->SetPercent(LobbyGS->PlayerSlot1.MaxPosture / 10);
		Bar_AttackDamage->SetPercent(LobbyGS->PlayerSlot1.AttackDamage / 10);
		Bar_PostureHealing->SetPercent(LobbyGS->PlayerSlot1.PostureHealing / 10);

		if (LobbyGS->PlayerSlot1.bIsReady)
		{
			Btn_Ready->SetVisibility(ESlateVisibility::Collapsed);
			Txt_Ready->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Btn_Ready->SetVisibility(ESlateVisibility::Collapsed);
			Txt_Ready->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		Txt_Nickname->SetText(FText::FromString(LobbyGS->PlayerSlot2.Nickname));
		IsPlayer01 = InIsPlayer01;

		Txt_MaxHp->SetText(FText::AsNumber(FMath::RoundToInt(LobbyGS->PlayerSlot2.MaxHp)));
		Txt_MaxPosture->SetText(FText::AsNumber(FMath::RoundToInt(LobbyGS->PlayerSlot2.MaxPosture)));
		Txt_AttackDamage->SetText(FText::AsNumber(FMath::RoundToInt(LobbyGS->PlayerSlot2.AttackDamage)));
		Txt_PostureHealing->SetText(FText::AsNumber(FMath::RoundToInt(LobbyGS->PlayerSlot2.PostureHealing)));
		Txt_RemaingPt->SetText(FText::AsNumber(LobbyGS->PlayerSlot2.RemainingPT));

		Bar_MaxHp->SetPercent(LobbyGS->PlayerSlot2.MaxHp / 10);
		Bar_MaxPosture->SetPercent(LobbyGS->PlayerSlot2.MaxPosture / 10);
		Bar_AttackDamage->SetPercent(LobbyGS->PlayerSlot2.AttackDamage / 10);
		Bar_PostureHealing->SetPercent(LobbyGS->PlayerSlot2.PostureHealing / 10);

		if (LobbyGS->PlayerSlot2.bIsReady)
		{
			Btn_Ready->SetVisibility(ESlateVisibility::Collapsed);
			Txt_Ready->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Btn_Ready->SetVisibility(ESlateVisibility::Collapsed);
			Txt_Ready->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if ((LobbyGS->PlayerSlot1.OwningPlayerState == GetOwningPlayerState() && InIsPlayer01) ||
		(LobbyGS->PlayerSlot2.OwningPlayerState == GetOwningPlayerState() && !InIsPlayer01))
	{
		Btn_Cancle->SetVisibility(ESlateVisibility::Visible);
		Btn_MaxHpDown->SetVisibility(ESlateVisibility::Visible);
		Btn_MaxHpUp->SetVisibility(ESlateVisibility::Visible);
		Btn_MaxPostureDown->SetVisibility(ESlateVisibility::Visible);
		Btn_MaxPostureUp->SetVisibility(ESlateVisibility::Visible);
		Btn_AttackDamageDown->SetVisibility(ESlateVisibility::Visible);
		Btn_AttackDamageUp->SetVisibility(ESlateVisibility::Visible);
		Btn_PostureHealingDown->SetVisibility(ESlateVisibility::Visible);
		Btn_PostureHealingUp->SetVisibility(ESlateVisibility::Visible);

		if (LobbyGS->PlayerSlot1.bIsReady && LobbyGS->PlayerSlot1.OwningPlayerState == GetOwningPlayerState())
		{
			Btn_Ready->SetVisibility(ESlateVisibility::Collapsed);
			Txt_Ready->SetVisibility(ESlateVisibility::Visible);
		}
		else if (LobbyGS->PlayerSlot2.bIsReady && LobbyGS->PlayerSlot2.OwningPlayerState == GetOwningPlayerState())
		{
			Btn_Ready->SetVisibility(ESlateVisibility::Collapsed);
			Txt_Ready->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Btn_Ready->SetVisibility(ESlateVisibility::Visible);
			Txt_Ready->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		Btn_Cancle->SetVisibility(ESlateVisibility::Collapsed);
		Btn_MaxHpDown->SetVisibility(ESlateVisibility::Collapsed);
		Btn_MaxHpUp->SetVisibility(ESlateVisibility::Collapsed);
		Btn_MaxPostureDown->SetVisibility(ESlateVisibility::Collapsed);
		Btn_MaxPostureUp->SetVisibility(ESlateVisibility::Collapsed);
		Btn_AttackDamageDown->SetVisibility(ESlateVisibility::Collapsed);
		Btn_AttackDamageUp->SetVisibility(ESlateVisibility::Collapsed);
		Btn_PostureHealingDown->SetVisibility(ESlateVisibility::Collapsed);
		Btn_PostureHealingUp->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGamePlayerListUI::OnClickedCancleGameButton()
{
	LobbyRoomObject->CanclePlayer(IsPlayer01);
}

void UGamePlayerListUI::OnClickedMaxHpDownGameButton()
{
	if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetOwningPlayerState()))
	{
		LobbyPS->C2S_UpdatePlayer(IsPlayer01, EGameStatType::MaxHp, -1);
	}
}

void UGamePlayerListUI::OnClickedMaxHpUpGameButton()
{
	if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetOwningPlayerState()))
	{
		LobbyPS->C2S_UpdatePlayer(IsPlayer01, EGameStatType::MaxHp, 1);
	}
}

void UGamePlayerListUI::OnClickedMaxPostureDownGameButton()
{
	if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetOwningPlayerState()))
	{
		LobbyPS->C2S_UpdatePlayer(IsPlayer01, EGameStatType::MaxPosture, -1);
	}
}

void UGamePlayerListUI::OnClickedMaxPostureUpGameButton()
{
	if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetOwningPlayerState()))
	{
		LobbyPS->C2S_UpdatePlayer(IsPlayer01, EGameStatType::MaxPosture, 1);
	}
}

void UGamePlayerListUI::OnClickedAttackDamageDownGameButton()
{
	if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetOwningPlayerState()))
	{
		LobbyPS->C2S_UpdatePlayer(IsPlayer01, EGameStatType::AttackDamage, -1);
	}
}

void UGamePlayerListUI::OnClickedAttackDamageUpGameButton()
{
	if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetOwningPlayerState()))
	{
		LobbyPS->C2S_UpdatePlayer(IsPlayer01, EGameStatType::AttackDamage, 1);
	}
}

void UGamePlayerListUI::OnClickedPostureHealingDownGameButton()
{
	if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetOwningPlayerState()))
	{
		LobbyPS->C2S_UpdatePlayer(IsPlayer01, EGameStatType::PostureHealing, -1);
	}
}

void UGamePlayerListUI::OnClickedPostureHealingUpGameButton()
{
	if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetOwningPlayerState()))
	{
		LobbyPS->C2S_UpdatePlayer(IsPlayer01, EGameStatType::PostureHealing, 1);
	}
}

void UGamePlayerListUI::OnClickedReadyGameButton()
{
	if (ALobbyPlayerState* LobbyPS = Cast<ALobbyPlayerState>(GetOwningPlayerState()))
	{
		LobbyPS->C2S_UpdatePlayer(IsPlayer01, EGameStatType::Ready, 0);
	}
}
