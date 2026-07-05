// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayerListUI.generated.h"

class ULobbyRoomUI;
class UTextBlock;
class ALobbyGameState;

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API UGamePlayerListUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ULobbyRoomUI> LobbyRoomObject;

	void SetLobbyRoomUI(ULobbyRoomUI* LobbyRoomUI);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_Nickname;

	ALobbyGameState* LobbyGS;

	void RefreshUI(bool InIsPlayer01);
};
