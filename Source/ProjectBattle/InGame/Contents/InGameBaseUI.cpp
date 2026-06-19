// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/InGameBaseUI.h"
#include "Components/ProgressBar.h"

void UInGameBaseUI::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar->SetPercent(1);
	PostureBar->SetPercent(0);
}

void UInGameBaseUI::RefreshPlayerStat(float NewHp, float MaxHp, float NewPosture, float MaxPosture)
{
	HpBar->SetPercent(NewHp / MaxHp);
	PostureBar->SetPercent(NewPosture / MaxPosture);

	PlayerData.CurrentHp = NewHp;
	PlayerData.MaxHp = MaxHp;
	PlayerData.CurrentPosture = NewPosture;
	PlayerData.MaxPosture = MaxPosture;
}

void UInGameBaseUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (PlayerData.CurrentHp != 0.0f)
	{
		PlayerPostureHeal = PostureHeal * InDeltaTime;
		PlayerHpRate = PlayerData.CurrentHp / PlayerData.MaxHp;

		if (PlayerHpRate >= 0.75f)
		{
			PlayerData.CurrentPosture = FMath::Clamp(PlayerData.CurrentPosture - PlayerPostureHeal, 0.0f, PlayerData.MaxPosture);
		}
		else if (PlayerHpRate >= 0.5 && PlayerHpRate < 0.75)
		{
			PlayerData.CurrentPosture = FMath::Clamp(PlayerData.CurrentPosture - (PlayerPostureHeal * 0.66f), 0.0f, PlayerData.MaxPosture);
		}
		else if (PlayerHpRate >= 0.25 && PlayerHpRate < 0.5)
		{
			PlayerData.CurrentPosture = FMath::Clamp(PlayerData.CurrentPosture - (PlayerPostureHeal * 0.33f), 0.0f, PlayerData.MaxPosture);
		}
		else
		{
			PlayerData.CurrentPosture = FMath::Clamp(PlayerData.CurrentPosture - (PlayerPostureHeal * 0.01f), 0.0f, PlayerData.MaxPosture);
		}

		PostureBar->SetPercent(PlayerData.CurrentPosture / PlayerData.MaxPosture);
	}
}
