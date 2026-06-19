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

	void RefreshPlayerStat(float NewHp, float MaxHp, float NewPosture, float MaxPosture);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UProgressBar> PostureBar;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	float PlayerHp = 0.0f;
	float PlayerMaxHp = 0.0f;
	float PlayerPosture = 0.0f;
	float PlayerMaxPosture = 0.0f;

	float PlayerHpRate;
	float PlayerPostureHeal;

	float EnemyHp = 0.0f;
	float EnemyMaxHp = 0.0f;
	float EnemyPosture = 0.0f;
	float EnemyMaxPosture = 0.0f;

	float PostureHeal = 3.0f;

};
