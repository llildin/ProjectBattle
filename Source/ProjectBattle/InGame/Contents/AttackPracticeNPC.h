// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGame/Contents/Human.h"

#include "AttackPracticeNPC.generated.h"

UENUM(BlueprintType)
enum class ENPCState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	On_Damaged = 10 UMETA(DisplayName = "On_Damaged"),
	Guard = 20 UMETA(DisplayName = "Guard"),
	BasicAttack = 30 UMETA(DisPlayName = "BasicAttack")
};

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API AAttackPracticeNPC : public AHuman
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//NPC Setting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ENPCState CurrentState = ENPCState::Idle;

};
