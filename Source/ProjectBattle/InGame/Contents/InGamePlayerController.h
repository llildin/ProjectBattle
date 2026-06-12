// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "InGamePlayerController.generated.h"

class UInputMappingContext;
class UNPCSetting;
class UInGameBaseUI;
class AInGamePlayer;

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API AInGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<TSoftObjectPtr<UInputMappingContext>> IMC_InGame;

protected:
	virtual void OnPossess(APawn* aPawn) override;

	virtual void OnUnPossess() override;

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UInGameBaseUI> InGameBaseUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UInGameBaseUI> InGameBaseUIObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UNPCSetting> NPCSettingClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UNPCSetting> NPCSettingObject;


	void NPCSettingInteract();

	AInGamePlayer* Player;
};
