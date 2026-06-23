// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleMainUI.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API UTitleMainUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_Start;

	UFUNCTION()
	void OnClickedStartButton();

	DECLARE_DELEGATE(FOnStartGame)
	FOnStartGame OnStartGame;
};
