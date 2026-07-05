// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameState.generated.h"

class ULobbyRoomUI;

USTRUCT(BlueprintType)
struct FGameSlotData
{
	GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TWeakObjectPtr<APlayerState> OwningPlayerState;

    UPROPERTY(BlueprintReadOnly)
    FString Nickname;

    UPROPERTY(BlueprintReadOnly)
    float MaxHp = 3.0f;

    UPROPERTY(BlueprintReadOnly)
    float MaxPosture = 3.0f;

    UPROPERTY(BlueprintReadOnly)
    float AttackDamage = 3.0f;

    UPROPERTY(BlueprintReadOnly)
    float PostureHealing = 3.0f;

    UPROPERTY(BlueprintReadOnly)
    int32 RemainingPT = 10;

	UPROPERTY(BlueprintReadOnly)
	bool bIsReady = false;
};

UENUM(BlueprintType)
enum class EGameStatType : uint8
{
    Join = 0 UMETA(DisplayName = "Join"),
    MaxHp = 10 UMETA(DisplayName = "MaxHp"),
    MaxPosture = 20 UMETA(DisplayName = "MaxPosture"),
    AttackDamage = 30 UMETA(DisplayName = "AttackDamage"),
    PostureHealing = 40 UMETA(DisplayName = "PostureHealing")
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

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<ULobbyRoomUI> LobbyRoomObject;

    void SetLobbyRoomUI(ULobbyRoomUI* LobbyRoomUI);

    void UpdateSlot(bool IsPlayer01, APlayerState* PlayerState, EGameStatType GameStatType, float UpDown);

};
