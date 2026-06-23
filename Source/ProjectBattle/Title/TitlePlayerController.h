// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "TitlePlayerController.generated.h"

class UTitleMainUI;
class ULobbyMainUI;

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API ATitlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UTitleMainUI> TitleMainUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UTitleMainUI> TitleMainUIObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<ULobbyMainUI> LobbyMainUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<ULobbyMainUI> LobbyMainUIObject;


};
