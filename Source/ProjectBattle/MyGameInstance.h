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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHp_Player01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHp_Player02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxPosture_Player01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxPosture_Player02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AttackDamage_Player01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AttackDamage_Player02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float PostureHealing_Player01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float PostureHealing_Player02;
	
	DECLARE_MULTICAST_DELEGATE(FOnLobbyListChanged);
	FOnLobbyListChanged OnLobbyListChanged;
};
