// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyRoomUI.generated.h"

class UVerticalBox;
class UObserberListUI;

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
};
