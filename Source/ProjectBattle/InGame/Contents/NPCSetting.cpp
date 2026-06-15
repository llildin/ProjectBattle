// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/NPCSetting.h"
#include "Components/Button.h"
#include "AttackPracticeNPC.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ComboBoxString.h"

void UNPCSetting::NativeConstruct()
{
	Super::NativeConstruct();

	NPC = Cast<AAttackPracticeNPC>(UGameplayStatics::GetActorOfClass(GetWorld(), AAttackPracticeNPC::StaticClass()));
	if (!NPC)
	{
		return;
	}

	if (Btn_Close)
	{
		Btn_Close->OnClicked.RemoveDynamic(this, &UNPCSetting::OnClickedCloseButton);
		Btn_Close->OnClicked.AddDynamic(this, &UNPCSetting::OnClickedCloseButton);
	}

	SelectStateBox->ClearOptions();
	SelectStateBox->AddOption(TEXT("Battle"));
	SelectStateBox->AddOption(TEXT("BasicAttack"));
	SelectStateBox->AddOption(TEXT("Guard"));

	SelectStateBox->SetSelectedOption(TEXT("Battle"));
}

void UNPCSetting::OnClickedCloseButton()
{
	OnCloseUI.ExecuteIfBound();

	FString SelectedState = SelectStateBox->GetSelectedOption();
	const UEnum* EnumPtr = StaticEnum<ECurrentState>();
	FString StateName = FString::Printf(TEXT("ECurrentState::%s"), *SelectedState);
	int64 EnumValue = EnumPtr->GetValueByName(FName(*StateName));

	NPC->SetState(static_cast<ECurrentState>(EnumValue));
}