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

//UENUM(BlueprintType)
//enum class EBattleState : uint8
//{
//
//};

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


	//Input 호출 함수
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

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
