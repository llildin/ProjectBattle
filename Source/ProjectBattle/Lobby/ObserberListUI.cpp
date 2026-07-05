// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/ObserberListUI.h"
#include "Components/TextBlock.h"

void UObserberListUI::SetNickname(FString InName)
{
	Txt_Nickname->SetText(FText::FromString(InName));
}
