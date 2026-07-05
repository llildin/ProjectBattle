// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObserberListUI.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API UObserberListUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_Nickname;

	void SetNickname(FString InName);
};
