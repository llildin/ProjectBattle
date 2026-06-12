// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCSetting.generated.h"

class UButton;
class UComboBoxString;
class AAttackPracticeNPC;

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API UNPCSetting : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_Close;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UComboBoxString> SelectStateBox;

	UFUNCTION()
	void OnClickedCloseButton();

	DECLARE_DELEGATE(FOnCloseUI)
	FOnCloseUI OnCloseUI;


	//Set NPC
	AAttackPracticeNPC* NPC;

};
