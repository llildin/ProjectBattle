// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "InGamePlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UStaticMeshComponent;

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Walk = 10 UMETA(DisplayName = "Walk"),
	Run = 20 UMETA(DisplayName = "Run")
};

UENUM(BlueprintType)
enum class ECurrentState : uint8
{
	No_Battle = 0 UMETA(DisplayName = "No_Battle"),
	Battle = 10 UMETA(DisplayName = "Battle"),
	Guard = 20 UMETA(DisplayName = "Guard"),
	Attack = 30 UMETA(DisplayName = "Attack"),
	On_Damaged = 40 UMETA(DisplayName = "On_Damaged"),
	Rolling = 50 UMETA(DisplayName = "Rolling")
};

UCLASS()
class PROJECTBATTLE_API AInGamePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInGamePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Katana;




	//Input 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_BasicAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_No_Battle;


	//Input 호출 함수
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void No_Battle(const FInputActionValue& Value);


	//Player 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ECurrentState CurrentState = ECurrentState::No_Battle;

	DECLARE_DELEGATE_OneParam(FOnStateChanged, ECurrentState)
	FOnStateChanged OnStateChanged;

	void SetCurrentState(ECurrentState NewState);

	UFUNCTION(BlueprintImplementableEvent)
	void BattleCameraSetting(FRotator StartRotator, FRotator EndRotator);


	//BasicCombo
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 BasicComboAttackCount = 0;

	UFUNCTION(BlueprintCallable)
	void BasicCheckComboAttack();

	UFUNCTION(BlueprintCallable)
	void BasicComboAttack();

	void PlayBasicComboAttackMontage();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<UAnimMontage> BasicComboAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 bIsBasicAttacking : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 PlayingBasicComboAttackIndex = 0;


};
