// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InGameBaseUI.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API UInGameBaseUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	void RefreshPlayerStat(float NewHp, float MaxHp);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;


};
