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
	SelectStateBox->AddOption(TEXT("Idle"));
	SelectStateBox->AddOption(TEXT("BasicAttack"));
	SelectStateBox->AddOption(TEXT("Guard"));

	SelectStateBox->SetSelectedOption(TEXT("Idle"));
}

void UNPCSetting::OnClickedCloseButton()
{
	OnCloseUI.ExecuteIfBound();

	FString SelectedState = SelectStateBox->GetSelectedOption();
	const UEnum* EnumPtr = StaticEnum<ENPCState>();
	FString StateName = FString::Printf(TEXT("ENPCState::%s"), *SelectedState);
	int64 EnumValue = EnumPtr->GetValueByName(FName(*StateName));

	NPC->CurrentState = static_cast<ENPCState>(EnumValue);
}