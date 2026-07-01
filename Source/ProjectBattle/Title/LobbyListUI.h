// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FindSessionsCallbackproxy.h"
#include "Blueprint/UserWidget.h"
#include "LobbyListUI.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API ULobbyListUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void SetInfo(FBlueprintSessionResult InSessionResult);

	UFUNCTION(BlueprintCallable)
	void RefreshUI();

	UPROPERTY(BlueprintReadOnly)
	FBlueprintSessionResult Result;


	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_RoomName;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> Txt_PlayerCount;
};
