// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Human.generated.h"

class UStaticMeshComponent;

#define HIT_TIME		3.0f
#define POSTUREHEAL		3.0f

UENUM(BlueprintType)
enum class ECurrentState : uint8
{
	No_Battle = 0 UMETA(DisplayName = "No_Battle"),
	Battle = 10 UMETA(DisplayName = "Battle"),
	Guard = 20 UMETA(DisplayName = "Guard"),
	Attack = 30 UMETA(DisplayName = "Attack"),
	BasicAttack = 31 UMETA(DisplayName = "BasicAttack"),
	On_Damaged = 40 UMETA(DisplayName = "On_Damaged"),
	Rolling = 50 UMETA(DisplayName = "Rolling"),
	Interact = 60 UMETA(DisplayName = "Interact")
};

UENUM(BlueprintType)
enum class EGuardHit : uint8
{
	Hit = 0 UMETA(DisplayName = "Hit"),
	Guard_Hit = 0 UMETA(DisplayName = "Guard_Hit"),
	Guard_Perfect_Hit = 0 UMETA(DisplayName = "Guard_Perfect_Hit")
};

UCLASS()
class PROJECTBATTLE_API AHuman : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHuman();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", ReplicatedUsing = OnRep_CurrentState)
	ECurrentState CurrentState;

	UFUNCTION()
	virtual void OnRep_CurrentState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ECurrentState PrevState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Katana;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	float HP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	float Posture = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxPosture = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	float HitTime = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	float PostureHeal = 3.0f;

	float HpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PostureDamage")
	float NormalPostureDamageRate = 0.7f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PostureDamage")
	float GuardPostureDamageRate = 0.3f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PostureDamage")
	float PerfectGuardPostureDamageRate = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 bIsAttacking : 1 = false;

	virtual void SetCurrentState(ECurrentState NewState);

	virtual void RefreshAttackSetting();

	// On_Damaged
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	bool CheckIsDamaged();


	// Guard
	float GuardStartTime;

	void CheckGuard(float Damage, AActor* Attacker);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<UAnimMontage> Guard_Hit_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<UAnimMontage> Guard_Perfect_Hit_Montage;
};

/*
스탯 : 체력(Hp), 체간(Posture), 체간회복량(PostureHeal), 체간데미지(PostureDamage)

체간 최소 0
체간 최대 100

가드를 안하고 피격시 피격데미지의 70% 만큼 체간피해
가드를 하고 피격시 피격데미지의 30%만큼 체간피해
퍼펙트 가드시 피격데미지의 0%만큼 체간피해
퍼펙트 가드시 공격자에게 피격데미지의 30%만큼 체간피해

가드를 하지 않은 상태 기준에서는
체간회복량은 초당 3 회복(PostureHeal 수치 만큼)
체간은 현재 체력이 100%~75%일 때 100% 속도로 자연 회복
      체력이 75%~50%일 때 66% 속도로 자연 회복
	  체력이 50%~25%일 때 33% 속도로 자연 회복
      체력이 25%~0%일 때 1% 속도로 자연 회복

피격시 3초간 체간은 회복되지 않음
*/