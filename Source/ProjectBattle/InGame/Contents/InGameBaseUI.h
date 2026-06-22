// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "InGameBaseUI.generated.h"

class UProgressBar;

USTRUCT(BlueprintType)
struct FStatData
{
	GENERATED_BODY()

	float CurrentHp = 0.0f;
	float MaxHp = 100.0f;
	float CurrentPosture = 0.0f;
	float MaxPosture = 100.0f;
};

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	FStatData PlayerData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	FStatData EnemyData;

	float PostureHeal = 3.0f;
	float PlayerPostureHeal;
	float PlayerHpRate;
	float HitTime = 3.0f;
};
