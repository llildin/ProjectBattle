// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InGameGameModeBase.generated.h"

class APlayerStart;
class AInGamePlayerState;

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API AInGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<APlayerController*> PlayingControllers;

	UPROPERTY()
	TArray<APlayerController*> SpectatorControllers;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void FindPlayerStarts();

	UPROPERTY()
	APlayerStart* PlayerStart1;

	UPROPERTY()
	APlayerStart* PlayerStart2;

	UPROPERTY()
	APlayerStart* PlayerStart3;

	void InitSpectators();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
};
