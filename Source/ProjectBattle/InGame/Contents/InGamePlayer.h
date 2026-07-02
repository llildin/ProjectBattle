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

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	//Input 호출 함수
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void No_Battle(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void C2S_No_Battle();
	bool C2S_No_Battle_Validate();
	void C2S_No_Battle_Implementation();

	void GuardStart(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void C2S_GuardStart();
	bool C2S_GuardStart_Validate();
	void C2S_GuardStart_Implementation();

	void GuardEnd(const FInputActionValue& Value);

	void Roll(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void C2S_Roll(ECurrentState InPrevState);
	bool C2S_Roll_Validate(ECurrentState InPrevState);
	void C2S_Roll_Implementation(ECurrentState InPrevState);

	void RunStart(const FInputActionValue& Value);

	void RunEnd(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void C2S_Run(EMoveState State);
	void C2S_Run_Implementation(EMoveState State);


	void Interact(const FInputActionValue& Value);


	//Player 상태

	virtual void OnRep_CurrentState() override;

	UFUNCTION(Server, Reliable)
	void C2S_SetCurrentState(ECurrentState NewState);
	bool C2S_SetCurrentState_Validate(ECurrentState NewState);
	void C2S_SetCurrentState_Implementation(ECurrentState NewState);

	UPROPERTY(ReplicatedUsing = OnRep_CurrentMoveState)
	EMoveState CurrentMoveState = EMoveState::Idle;

	UFUNCTION()
	void OnRep_CurrentMoveState();

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

	UFUNCTION(Server, Reliable)
	void C2S_BasicCheckComboAttack(FRotator CameraRotation);
	void C2S_BasicCheckComboAttack_Implementation(FRotator CameraRotation);

	UFUNCTION(BlueprintCallable)
	void BasicComboAttack();

	UFUNCTION(Server, Reliable)
	void C2S_BasicComboAttack(FRotator CameraRotation, ECurrentState InCurrentState);
	void C2S_BasicComboAttack_Implementation(FRotator CameraRotation, ECurrentState InCurrentState);

	UFUNCTION(Server, Reliable)
	void C2S_AddComboCount();
	void C2S_AddComboCount_Implementation();

	void PlayBasicComboAttackMontage();

	UFUNCTION(NetMulticast, Reliable)
	void S2C_PlayBasicComboAttackMontage(FName SectionName);
	void S2C_PlayBasicComboAttackMontage_Implementation(FName SectionName);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<UAnimMontage> BasicComboAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stat")
	uint8 PlayingBasicComboAttackIndex = 0;

	FString AttackSectionName;

	virtual void RefreshAttackSetting() override;


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

	UFUNCTION(NetMulticast, Reliable)
	void S2C_StopAttackMontage();
	void S2C_StopAttackMontage_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2C_PlayRollingMontage(FName SectionName);
	void S2C_PlayRollingMontage_Implementation(FName SectionName);

	FName GetRollingSectionName(float Direction);

	FVector LastInputVector;


	//BasicAttackTrace

	void BasicAttackTrace();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> DT_AttackData;


	//NPC Setting
	bool bIsNPCSetting = false;

	//Damage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	void UpDateUI();
};
