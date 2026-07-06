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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat", Replicated)
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

	UFUNCTION(NetMulticast, Reliable)
	void S2C_TakeDamage(float ActualDamage);
	void S2C_TakeDamage_Implementation(float ActualDamage);

	bool CheckIsDamaged();


	// Guard
	float GuardStartTime;

	void CheckGuard(float Damage, AActor* Attacker);

	UFUNCTION(NetMulticast, Reliable)
	void S2C_CheckGuard(float CurrentTime, float MontageLength, float Damage, AActor* Attacker);
	void S2C_CheckGuard_Implementation(float CurrentTime, float MontageLength, float Damage, AActor* Attacker);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<UAnimMontage> Guard_Hit_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<UAnimMontage> Guard_Perfect_Hit_Montage;

	UFUNCTION(BlueprintImplementableEvent)
	void FinishGameWinner(const FString& Winner);

	UFUNCTION(NetMulticast, Reliable)
	void S2C_Winner(AActor* DamageCauser);
	void S2C_Winner_Implementation(AActor* DamageCauser);
};
