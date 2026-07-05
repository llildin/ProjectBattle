// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyRoomUI.generated.h"

class UVerticalBox;
class UObserberListUI;
class UJoinGameUI;
class UGamePlayerListUI;
class UBorder;

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API ULobbyRoomUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UVerticalBox> ObserverList;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void RefreshUI();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UObserberListUI> ObserberListUIClass;

	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<UObserberListUI>> ObserberListUIObject;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UJoinGameUI> JoinGameUIClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UJoinGameUI> JoinGameUIObject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGamePlayerListUI> Player01_GamePlayerListUIClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UGamePlayerListUI> Player01_GamePlayerListUIObject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGamePlayerListUI> Player02_GamePlayerListUIClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UGamePlayerListUI> Player02_GamePlayerListUIObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UBorder> Player01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (BindWidget))
	TObjectPtr<UBorder> Player02;
};
