// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/TitleMainUI.h"
#include "Components/Button.h"

void UTitleMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Start)
	{
		Btn_Start->OnClicked.RemoveDynamic(this, &UTitleMainUI::OnClickedStartButton);
		Btn_Start->OnClicked.AddDynamic(this, &UTitleMainUI::OnClickedStartButton);
	}
}

void UTitleMainUI::OnClickedStartButton()
{
	OnStartGame.ExecuteIfBound();


}
