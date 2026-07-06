// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "InGamePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API AInGamePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
    UPROPERTY(Replicated, BlueprintReadOnly)
    FString Nickname;

    UPROPERTY(Replicated, BlueprintReadOnly)
    float Stat_MaxHp;

    UPROPERTY(Replicated, BlueprintReadOnly)
    float Stat_MaxPosture;

    UPROPERTY(Replicated, BlueprintReadOnly)
    float Stat_AttackDamage;

    UPROPERTY(Replicated, BlueprintReadOnly)
    float Stat_PostureHealing;

    UPROPERTY(Replicated, BlueprintReadOnly)
    bool IsPlayer = false;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
