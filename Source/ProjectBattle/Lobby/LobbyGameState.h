// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameState.generated.h"

USTRUCT(BlueprintType)
struct FGameSlotData
{
	GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    bool bIsOccupied = false;

    UPROPERTY(BlueprintReadOnly)
    TWeakObjectPtr<APlayerState> OwningPlayerState;

    UPROPERTY(BlueprintReadOnly)
    FString Nickname;

    UPROPERTY(BlueprintReadOnly)
    int32 MaxHp = 3;

    UPROPERTY(BlueprintReadOnly)
    int32 MaxPosture = 3;

    UPROPERTY(BlueprintReadOnly)
    int32 AttackDamage = 3;

    UPROPERTY(BlueprintReadOnly)
    int32 PostureHealing = 3;

    UPROPERTY(BlueprintReadOnly)
    int32 RemainingPT = 10;

	UPROPERTY(BlueprintReadOnly)
	bool bIsReady = false;
};

UENUM(BlueprintType)
enum class EGameStatType : uint8
{
    MaxHp = 0 UMETA(DisplayName = "MaxHp"),
    MaxPosture = 10 UMETA(DisplayName = "MaxPosture"),
    AttackDamage = 20 UMETA(DisplayName = "AttackDamage"),
    PostureHealing = 30 UMETA(DisplayName = "PostureHealing")
};

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerCount)
	int32 PlayerCount = 0;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_PlayerCount();

	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;

    UPROPERTY(ReplicatedUsing = OnRep_PlayerSlot1, BlueprintReadOnly)
    FGameSlotData PlayerSlot1;

    UPROPERTY(ReplicatedUsing = OnRep_PlayerSlot2, BlueprintReadOnly)
    FGameSlotData PlayerSlot2;

    UFUNCTION() void OnRep_PlayerSlot1();
    UFUNCTION() void OnRep_PlayerSlot2();
};
