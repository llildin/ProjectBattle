// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InGame/Contents/Human.h"
#include "InputActionValue.h"
#include "Sturcture_AttackData.h"

#include "InGamePlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UStaticMeshComponent;
class AInGamePlayerController;

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	Walk = 10 UMETA(DisplayName = "Walk"),
	Run = 20 UMETA(DisplayName = "Run")
};

UCLASS()
class PROJECTBATTLE_API AInGamePlayer : public AHuman
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Guard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Roll;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Run;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Interact;

	
	AInGamePlayerController* Controller;


	//Input 호출 함수
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void No_Battle(const FInputActionValue& Value);

	void GuardStart(const FInputActionValue& Value);

	void GuardEnd(const FInputActionValue& Value);

	void Roll(const FInputActionValue& Value);

	void RunStart(const FInputActionValue& Value);

	void RunEnd(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);


	//Player 상태

	EMoveState CurrentMoveState = EMoveState::Idle;

	DECLARE_DELEGATE_OneParam(FOnStateChanged, ECurrentState)
	FOnStateChanged OnStateChanged;

	virtual void SetCurrentState(ECurrentState NewState) override;

	void UpdateMoveSpeed();

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

	FString AttackSectionName;


	//Test
	FRotator TargetAttackRotation;

	bool bIsComboRotating = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	float ComboRotationSpeed = 0;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);


	//Rolling
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<UAnimMontage> RollingMontage;

	void Rolling();

	FName GetRollingSectionName(float Direction);

	FVector LastInputVector;


	//BasicAttackTrace

	void BasicAttackTrace();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> DT_AttackData;


	//NPC Setting
	bool bIsNPCSetting = false;

	//On_Damaged
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

};
