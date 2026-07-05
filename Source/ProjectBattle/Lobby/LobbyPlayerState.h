// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User", Replicated)
	FString Nickname;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void CopyProperties(APlayerState* NewPlayerState) override;

	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SetNickname(const FString& InName);
	bool C2S_SetNickname_Validate(const FString& InName);
	void C2S_SetNickname_Implementation(const FString& InName);
};
