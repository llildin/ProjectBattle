// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGame/Contents/Human.h"
#include "Sturcture_AttackData.h"

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

	//NPC Setting

	virtual void SetCurrentState(ECurrentState NewState) override;

	virtual void RefreshAttackSetting() override;

	DECLARE_DELEGATE_OneParam(FOnStateChanged, ECurrentState)
	FOnStateChanged OnStateChanged;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	int32 NPCComboCount = 0;
	FString AttackSectionName;

	void StartNPCBasicComboAttack();

	void NPCCheckBasicComboAttack();

	void PlayNPCBasicComboAttackMontage();

	void NPCBasicAttackTrace();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> DT_AttackData;

};
