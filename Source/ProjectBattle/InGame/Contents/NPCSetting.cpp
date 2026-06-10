// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/NPCSetting.h"
#include "Components/Button.h"

void UNPCSetting::NativeConstruct()
{
	if (Btn_Close)
	{
		Btn_Close->OnClicked.RemoveDynamic(this, &UNPCSetting::OnClickedCloseButton);
		Btn_Close->OnClicked.AddDynamic(this, &UNPCSetting::OnClickedCloseButton);
	}
}

void UNPCSetting::OnClickedCloseButton()
{
	OnCloseUI.ExecuteIfBound();
}