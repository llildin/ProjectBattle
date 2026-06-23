// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FindSessionsCallbackproxy.h"
#include "Blueprint/UserWidget.h"
#include "LobbyMainUI.generated.h"

class UButton;
class UVerticalBox;
class ULobbyListUI;

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API ULobbyMainUI : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_Create;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_Refresh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UButton> Btn_Quit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UVerticalBox> RoomList;

	UFUNCTION(BlueprintImplementableEvent)
	void SetSessionInfos();

	UFUNCTION(BlueprintCallable)
	void SetInfo();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ULobbyListUI> LobbyListUIClass;

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<ULobbyListUI>> LobbyListUIObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session")
	TArray<FBlueprintSessionResult> SessionInfos;

	UFUNCTION(BlueprintCallable)
	void RefreshUI();

	UFUNCTION()
	void OnClickedCreateButton();

	UFUNCTION(BlueprintImplementableEvent)
	void OpenCreateRoomUI();

	UFUNCTION()
	void OnClickedRefreshButton();

	UFUNCTION()
	void OnClickedQuitButton();

	DECLARE_DELEGATE(FOnQuitLobby)
	FOnQuitLobby OnQuitLobby;

};
