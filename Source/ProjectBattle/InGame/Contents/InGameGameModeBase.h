// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InGameGameModeBase.generated.h"

class APlayerStart;
/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API AInGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void FindPlayerStarts();

	UPROPERTY()
	TArray<TObjectPtr<APlayerController>> PlayingControllers;

	UPROPERTY()
	TArray<TObjectPtr<APlayerController>> SpectatorControllers;

	UPROPERTY()
	TObjectPtr<APlayerStart> PlayerStart1;

	UPROPERTY()
	TObjectPtr<APlayerStart> PlayerStart2;

	void RefreshSpectatorViewTargets();
};
