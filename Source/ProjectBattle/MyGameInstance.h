// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User")
	FString Nickname;
	
	DECLARE_MULTICAST_DELEGATE(FOnLobbyListChanged);
	FOnLobbyListChanged OnLobbyListChanged;
};
