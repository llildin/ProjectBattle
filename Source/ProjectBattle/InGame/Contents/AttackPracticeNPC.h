// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGame/Contents/Human.h"

#include "AttackPracticeNPC.generated.h"

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

};
