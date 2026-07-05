// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JoinGameUI.generated.h"

class UButton;
class ULobbyRoomUI;

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API UJoinGameUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ULobbyRoomUI> LobbyRoomObject;

	void SetLobbyRoomUI(ULobbyRoomUI* LobbyRoomUI, bool InIsPlayer01);

	bool IsPlayer01 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_JoinGame;

	UFUNCTION()
	void OnClickedJoinGameButton();
};
