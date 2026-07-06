// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayerListUI.generated.h"

class ULobbyRoomUI;
class UTextBlock;
class ALobbyGameState;
class UButton;
class UProgressBar;

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API UGamePlayerListUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ULobbyRoomUI> LobbyRoomObject;

	void SetLobbyRoomUI(ULobbyRoomUI* LobbyRoomUI);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_Nickname;

	ALobbyGameState* LobbyGS;

	bool IsPlayer01 = false;

	void RefreshUI(bool InIsPlayer01);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_Cancle;

	UFUNCTION()
	void OnClickedCancleGameButton();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_MaxHpDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_MaxHpUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_MaxPostureDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_MaxPostureUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_AttackDamageDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_AttackDamageUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_PostureHealingDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_PostureHealingUp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_MaxPosture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_PostureHealing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_RemaingPt;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UProgressBar> Bar_MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UProgressBar> Bar_MaxPosture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UProgressBar> Bar_AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UProgressBar> Bar_PostureHealing;

	UFUNCTION()
	void OnClickedMaxHpDownGameButton();

	UFUNCTION()
	void OnClickedMaxHpUpGameButton();

	UFUNCTION()
	void OnClickedMaxPostureDownGameButton();

	UFUNCTION()
	void OnClickedMaxPostureUpGameButton();

	UFUNCTION()
	void OnClickedAttackDamageDownGameButton();

	UFUNCTION()
	void OnClickedAttackDamageUpGameButton();

	UFUNCTION()
	void OnClickedPostureHealingDownGameButton();

	UFUNCTION()
	void OnClickedPostureHealingUpGameButton();
};
