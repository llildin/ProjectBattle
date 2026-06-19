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

	PlayerHp = NewHp;
	PlayerMaxHp = MaxHp;
	PlayerPosture = NewPosture;
	PlayerMaxPosture = MaxPosture;
}

void UInGameBaseUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	PlayerPostureHeal = PostureHeal * InDeltaTime;
	PlayerHpRate = PlayerHp / PlayerMaxHp;

	if (PlayerHpRate >= 0.75f)
	{
		PlayerPosture = FMath::Clamp(PlayerPosture - PlayerPostureHeal, 0.0f, PlayerMaxPosture);
	}
	else if (PlayerHpRate >= 0.5 && PlayerHpRate < 0.75)
	{
		PlayerPosture = FMath::Clamp(PlayerPosture - (PlayerPostureHeal * 0.66f), 0.0f, PlayerMaxPosture);
	}
	else if (PlayerHpRate >= 0.25 && PlayerHpRate < 0.5)
	{
		PlayerPosture = FMath::Clamp(PlayerPosture - (PlayerPostureHeal * 0.33f), 0.0f, PlayerMaxPosture);
	}
	else
	{
		PlayerPosture = FMath::Clamp(PlayerPosture - (PlayerPostureHeal * 0.01f), 0.0f, PlayerMaxPosture);
	}

	PostureBar->SetPercent(PlayerPosture / PlayerMaxPosture);
}
