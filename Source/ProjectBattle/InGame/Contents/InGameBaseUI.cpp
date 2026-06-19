// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/InGameBaseUI.h"
#include "Components/ProgressBar.h"

void UInGameBaseUI::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar->SetPercent(1);
}

void UInGameBaseUI::RefreshPlayerStat(float NewHp, float MaxHp)
{
	HpBar->SetPercent(NewHp / MaxHp);
}