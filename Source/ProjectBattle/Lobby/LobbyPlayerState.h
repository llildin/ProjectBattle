// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

UENUM(BlueprintType)
enum class EPlayerRole : uint8
{
	Spectator,
	Playing
};

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(ReplicatedUsing = OnRep_Nickname, EditAnywhere, BlueprintReadWrite, Category = "User")
	FString Nickname;

	UFUNCTION()
	void OnRep_Nickname();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void CopyProperties(APlayerState* NewPlayerState) override;

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SetNickname(const FString& InName);
	bool C2S_SetNickname_Validate(const FString& InName);
	void C2S_SetNickname_Implementation(const FString& InName);

	UPROPERTY(ReplicatedUsing = OnRep_PlayerRole, BlueprintReadOnly, Category = "User")
	EPlayerRole PlayerRole = EPlayerRole::Spectator;

	UFUNCTION()
	void OnRep_PlayerRole();

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_JoinGame();
	bool C2S_JoinGame_Validate();
	void C2S_JoinGame_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_JoinPlayer(bool InIsPlayer01);
	bool C2S_JoinPlayer_Validate(bool InIsPlayer01);
	void C2S_JoinPlayer_Implementation(bool InIsPlayer01);

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_UpdatePlayer(bool InIsPlayer01, EGameStatType InGameStatType, float InUpDown);
	bool C2S_UpdatePlayer_Validate(bool InIsPlayer01, EGameStatType InGameStatType, float InUpDown);
	void C2S_UpdatePlayer_Implementation(bool InIsPlayer01, EGameStatType InGameStatType, float InUpDown);

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_CanclePlayer(bool InIsPlayer01);
	bool C2S_CanclePlayer_Validate(bool InIsPlayer01);
	void C2S_CanclePlayer_Implementation(bool InIsPlayer01);

	UFUNCTION(NetMulticast, Reliable)
	void S2C_CanclePlayer(bool InIsPlayer01);
	void S2C_CanclePlayer_Implementation(bool InIsPlayer01);
};
