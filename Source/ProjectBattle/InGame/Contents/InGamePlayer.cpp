// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/InGamePlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AInGamePlayer::AInGamePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Katana = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Katana"));
	Katana->SetupAttachment(GetMesh(), TEXT("Katana_Socket"));

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()),
		FRotator(0, -90.f, 0)
	);

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

// Called when the game starts or when spawned
void AInGamePlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInGamePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AInGamePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (UIC)
	{
		UIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AInGamePlayer::Move);
		UIC->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AInGamePlayer::Look);
		UIC->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &AInGamePlayer::Jump);
		UIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AInGamePlayer::StopJumping);
		UIC->BindAction(IA_Jump, ETriggerEvent::Canceled, this, &AInGamePlayer::StopJumping);

		UIC->BindAction(IA_BasicAttack, ETriggerEvent::Started, this, &AInGamePlayer::BasicComboAttack);
		UIC->BindAction(IA_No_Battle, ETriggerEvent::Started, this, &AInGamePlayer::No_Battle);
	}
}

void AInGamePlayer::Move(const FInputActionValue& Value)
{
	FVector2D Direction = Value.Get<FVector2D>();

	FRotator CameraRotation = GetControlRotation();

	FRotator CameraRotaitionInFloor = FRotator(0, CameraRotation.Yaw, 0);

	FVector CameraForwardInFloor = UKismetMathLibrary::GetForwardVector(CameraRotaitionInFloor);

	FVector CameraRightInFloor = UKismetMathLibrary::GetRightVector(CameraRotaitionInFloor);

	AddMovementInput(CameraForwardInFloor * Direction.X);

	AddMovementInput(CameraRightInFloor * Direction.Y);
}

void AInGamePlayer::Look(const FInputActionValue& Value)
{
	FVector2D RotationDirection = Value.Get<FVector2D>();

	AddControllerPitchInput(RotationDirection.Y);
	AddControllerYawInput(RotationDirection.X);
}

void AInGamePlayer::No_Battle(const FInputActionValue& Value)
{
	if (CurrentState == ECurrentState::Battle)
	{
		SetCurrentState(ECurrentState::No_Battle);
	}
}

void AInGamePlayer::SetCurrentState(ECurrentState NewState)
{
	CurrentState = NewState;
	OnStateChanged.ExecuteIfBound(NewState);
}

void AInGamePlayer::BasicCheckComboAttack()
{
	if (PlayingBasicComboAttackIndex != BasicComboAttackCount)
	{
		PlayBasicComboAttackMontage();
		PlayingBasicComboAttackIndex = BasicComboAttackCount;
	}
}

void AInGamePlayer::BasicComboAttack()
{
	if (CurrentState == ECurrentState::No_Battle || CurrentState == ECurrentState::Battle || CurrentState == ECurrentState::Attack)
	{
		if (!bIsBasicAttacking)
		{
			BasicComboAttackCount++;

			PlayBasicComboAttackMontage();

			bIsBasicAttacking = true;

			SetCurrentState(ECurrentState::Attack);

			PlayingBasicComboAttackIndex = BasicComboAttackCount;
		}
		else if (bIsBasicAttacking && PlayingBasicComboAttackIndex == BasicComboAttackCount)
		{
			BasicComboAttackCount++;
		}
	}
}

void AInGamePlayer::PlayBasicComboAttackMontage()
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		FString SectionName = FString::Printf(TEXT("BasicAttack0%d"), BasicComboAttackCount);

		float MontageLength = PlayAnimMontage(BasicComboAttackMontage, 1.0f, FName(SectionName));

		if (MontageLength > 0)
		{
			FOnMontageEnded EndDelegate;

			EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted) {
				if (!bInterrupted)
				{
					BasicComboAttackCount = 0;
					PlayingBasicComboAttackIndex = 0;
					bIsBasicAttacking = false;
					SetCurrentState(ECurrentState::Battle);
				}
				});
			AnimInstance->Montage_SetEndDelegate(EndDelegate);
		}
	}
}

