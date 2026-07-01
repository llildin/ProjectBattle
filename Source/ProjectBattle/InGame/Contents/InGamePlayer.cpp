// Fill out your copyright notice in the Description page of Project Settings.

#include "InGame/Contents/InGamePlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "InGamePlayerController.h"
#include "InGame/AttackFunction.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AInGamePlayer::AInGamePlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	CurrentState = ECurrentState::No_Battle;
	PrevState = ECurrentState::No_Battle;

	GetCharacterMovement()->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AInGamePlayer::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())  // 로컬 플레이어만
	{
		Controller = Cast<AInGamePlayerController>(GetController());
	}
}

// Called every frame
void AInGamePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (bIsComboRotating)
		{
			FRotator CurrentRot = GetActorRotation();

			FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetAttackRotation, DeltaTime, ComboRotationSpeed);
			SetActorRotation(NewRot);

			if (CurrentRot.Equals(TargetAttackRotation, 1.0f))
			{
				bIsComboRotating = false;
			}
		}
	}

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

		UIC->BindAction(IA_Guard, ETriggerEvent::Started, this, &AInGamePlayer::GuardStart);
		UIC->BindAction(IA_Guard, ETriggerEvent::Completed, this, &AInGamePlayer::GuardEnd);

		UIC->BindAction(IA_Roll, ETriggerEvent::Started, this, &AInGamePlayer::Roll);
		UIC->BindAction(IA_Run, ETriggerEvent::Started, this, &AInGamePlayer::RunStart);
		UIC->BindAction(IA_Run, ETriggerEvent::Completed, this, &AInGamePlayer::RunEnd);

		UIC->BindAction(IA_Interact, ETriggerEvent::Started, this, &AInGamePlayer::Interact);
	}
}

void AInGamePlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AInGamePlayer, CurrentMoveState);
}

void AInGamePlayer::Move(const FInputActionValue& Value)
{
	if (CurrentState == ECurrentState::No_Battle || CurrentState == ECurrentState::Battle || CurrentState == ECurrentState::Guard)
	{
		FVector2D Direction = Value.Get<FVector2D>();

		FRotator CameraRotation = GetControlRotation();

		FRotator CameraRotaitionInFloor = FRotator(0, CameraRotation.Yaw, 0);

		FVector CameraForwardInFloor = UKismetMathLibrary::GetForwardVector(CameraRotaitionInFloor);

		FVector CameraRightInFloor = UKismetMathLibrary::GetRightVector(CameraRotaitionInFloor);

		LastInputVector = CameraForwardInFloor * Direction.X + CameraRightInFloor * Direction.Y;

		AddMovementInput(CameraForwardInFloor * Direction.X);

		AddMovementInput(CameraRightInFloor * Direction.Y);
	}
	else if (CurrentState == ECurrentState::Attack)
	{
		FVector2D Direction = Value.Get<FVector2D>();

		FRotator CameraRotation = GetControlRotation();

		FRotator CameraRotaitionInFloor = FRotator(0, CameraRotation.Yaw, 0);

		FVector CameraForwardInFloor = UKismetMathLibrary::GetForwardVector(CameraRotaitionInFloor);

		FVector CameraRightInFloor = UKismetMathLibrary::GetRightVector(CameraRotaitionInFloor);

		LastInputVector = CameraForwardInFloor * Direction.X + CameraRightInFloor * Direction.Y;
	}
}

void AInGamePlayer::Look(const FInputActionValue& Value)
{
	FVector2D RotationDirection = Value.Get<FVector2D>();

	AddControllerPitchInput(RotationDirection.Y);
	AddControllerYawInput(RotationDirection.X);
}

void AInGamePlayer::No_Battle(const FInputActionValue& Value)
{
	if (CurrentState == ECurrentState::Battle || CurrentState == ECurrentState::Guard)
	{
		C2S_No_Battle();
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

bool AInGamePlayer::C2S_No_Battle_Validate()
{
	return true;
}

void AInGamePlayer::C2S_No_Battle_Implementation()
{
	SetCurrentState(ECurrentState::No_Battle);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AInGamePlayer::GuardStart(const FInputActionValue& Value)
{
	if (CurrentState == ECurrentState::Battle)
	{
		C2S_GuardStart();
	}
}

bool AInGamePlayer::C2S_GuardStart_Validate()
{
	return true;
}

void AInGamePlayer::C2S_GuardStart_Implementation()
{
	GuardStartTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	SetCurrentState(ECurrentState::Guard);
}

void AInGamePlayer::GuardEnd(const FInputActionValue& Value)
{
	if (CurrentState == ECurrentState::Guard)
	{
		C2S_SetCurrentState(ECurrentState::Battle);
	}
}

void AInGamePlayer::Roll(const FInputActionValue& Value)
{
	if (CurrentState == ECurrentState::No_Battle)
	{
		C2S_Roll(CurrentState);
	}
	else if (CurrentState != ECurrentState::Rolling && CurrentState != ECurrentState::On_Damaged)
	{
		C2S_Roll(ECurrentState::Battle);
	}
}

bool AInGamePlayer::C2S_Roll_Validate(ECurrentState InPrevState)
{
	return true;
}

void AInGamePlayer::C2S_Roll_Implementation(ECurrentState InPrevState)
{
	PrevState = InPrevState;
	SetCurrentState(ECurrentState::Rolling);
	Rolling();
}

void AInGamePlayer::RunStart(const FInputActionValue& Value)
{
	C2S_RunStart();
}

bool AInGamePlayer::C2S_RunStart_Validate()
{
	return true;
}

void AInGamePlayer::C2S_RunStart_Implementation()
{
	CurrentMoveState = EMoveState::Run;
	UpdateMoveSpeed();
}

void AInGamePlayer::RunEnd(const FInputActionValue& Value)
{
	C2S_RunEnd();
}

bool AInGamePlayer::C2S_RunEnd_Validate()
{
	return true;
}

void AInGamePlayer::C2S_RunEnd_Implementation()
{
	CurrentMoveState = EMoveState::Idle;
	UpdateMoveSpeed();
}

void AInGamePlayer::Interact(const FInputActionValue& Value)
{
	if (bIsNPCSetting && (CurrentState == ECurrentState::No_Battle || CurrentState == ECurrentState::Battle))
	{
		PrevState = CurrentState;
		Controller->NPCSettingInteract();
	}
}

void AInGamePlayer::OnRep_CurrentState()
{
	Super::OnRep_CurrentState();
	OnStateChanged.ExecuteIfBound(CurrentState);
	UpdateMoveSpeed();
}

bool AInGamePlayer::C2S_SetCurrentState_Validate(ECurrentState NewState)
{
	return true;
}

void AInGamePlayer::C2S_SetCurrentState_Implementation(ECurrentState NewState)
{
	SetCurrentState(NewState);
}

void AInGamePlayer::SetCurrentState(ECurrentState NewState)
{
	Super::SetCurrentState(NewState);

	CurrentState = NewState;
	OnStateChanged.ExecuteIfBound(NewState);
	UpdateMoveSpeed();
}

void AInGamePlayer::UpdateMoveSpeed()
{
	switch (CurrentState)
	{
	case ECurrentState::No_Battle:
		if (CurrentMoveState == EMoveState::Idle)
		{
			GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		}
		else if (CurrentMoveState == EMoveState::Run)
		{
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		}
		break;

	case ECurrentState::Battle:
		if (CurrentMoveState == EMoveState::Idle)
		{
			GetCharacterMovement()->MaxWalkSpeed = 225.0f;
		}
		else if (CurrentMoveState == EMoveState::Run)
		{
			GetCharacterMovement()->MaxWalkSpeed = 450.0f;
		}
		break;

	case ECurrentState::Guard:
		GetCharacterMovement()->MaxWalkSpeed = 150.0f;
		break;
	}
}

void AInGamePlayer::BasicCheckComboAttack()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	if (PlayingBasicComboAttackIndex != BasicComboAttackCount)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &AInGamePlayer::OnAttackMontageEnded);
			AnimInstance->OnMontageEnded.AddDynamic(this, &AInGamePlayer::OnAttackMontageEnded);
		}

		FRotator CameraRot = GetControlRotation();
		C2S_BasicCheckComboAttack(CameraRot);
	}
}

void AInGamePlayer::C2S_BasicCheckComboAttack_Implementation(FRotator CameraRotation)
{
	TargetAttackRotation = FRotator(0.f, CameraRotation.Yaw, 0.f);
	bUseControllerRotationYaw = false;
	bIsComboRotating = true;

	PlayBasicComboAttackMontage();
	PlayingBasicComboAttackIndex = BasicComboAttackCount;
}

void AInGamePlayer::BasicComboAttack()
{
	if (CurrentState == ECurrentState::No_Battle || CurrentState == ECurrentState::Battle 
		|| CurrentState == ECurrentState::Attack || CurrentState == ECurrentState::Guard)
	{
		if (!bIsAttacking)
		{
			if (CurrentState == ECurrentState::No_Battle)
			{
				FRotator StartRotator = GetControlRotation();
				FRotator EndRotator = GetActorRotation();
				EndRotator.Pitch = -40.0f;
				EndRotator.Roll = 0.0f;
				BattleCameraSetting(StartRotator, EndRotator);
				GetCharacterMovement()->bOrientRotationToMovement = false;
			}

			C2S_BasicComboAttack(GetControlRotation(), CurrentState);
		}
		else if (bIsAttacking && PlayingBasicComboAttackIndex == BasicComboAttackCount)
		{
			C2S_AddComboCount();
		}
	}
}

void AInGamePlayer::C2S_BasicComboAttack_Implementation(FRotator CameraRotation, ECurrentState InCurrentState)
{
	BasicComboAttackCount++;
	PlayBasicComboAttackMontage();
	bIsAttacking = true;

	if (InCurrentState == ECurrentState::No_Battle)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	SetCurrentState(ECurrentState::Attack);
	PlayingBasicComboAttackIndex = BasicComboAttackCount;
}

void AInGamePlayer::C2S_AddComboCount_Implementation()
{
	BasicComboAttackCount++;
}

void AInGamePlayer::PlayBasicComboAttackMontage()
{
	AttackSectionName = FString::Printf(TEXT("BasicAttack0%d"), BasicComboAttackCount);
	S2C_PlayBasicComboAttackMontage(FName(AttackSectionName));
}

void AInGamePlayer::S2C_PlayBasicComboAttackMontage_Implementation(FName SectionName)
{
	AttackSectionName = SectionName.ToString();

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		float MontageLength = PlayAnimMontage(BasicComboAttackMontage, 1.0f, SectionName);
		if (MontageLength > 0)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindLambda([WeakThis = TWeakObjectPtr<AInGamePlayer>(this)](UAnimMontage* Montage, bool bInterrupted) {
				if (!bInterrupted && WeakThis.IsValid())
				{
					if (WeakThis->HasAuthority())
					{
						WeakThis->BasicComboAttackCount = 0;
						WeakThis->PlayingBasicComboAttackIndex = 0;
						WeakThis->bIsAttacking = false;
						WeakThis->bUseControllerRotationYaw = false;
						WeakThis->SetCurrentState(ECurrentState::Battle);
					}
				}
				});
			AnimInstance->Montage_SetEndDelegate(EndDelegate);
		}
	}
}

void AInGamePlayer::RefreshAttackSetting()
{
	if (!HasAuthority())
	{
		return;
	}

	Super::RefreshAttackSetting();

	BasicComboAttackCount = 0;
	PlayingBasicComboAttackIndex = 0;
	bIsAttacking = false;
}

void AInGamePlayer::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bUseControllerRotationYaw = true;
	bIsComboRotating = false;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &AInGamePlayer::OnAttackMontageEnded);
	}
}

void AInGamePlayer::Rolling()
{
	if (bIsAttacking)
	{
		S2C_StopAttackMontage();
		BasicComboAttackCount = 0;
		PlayingBasicComboAttackIndex = 0;
		bIsAttacking = false;
	}

	float Direction = UKismetAnimationLibrary::CalculateDirection(LastInputVector, GetActorRotation());
	FName SectionName = GetRollingSectionName(Direction);
	S2C_PlayRollingMontage(SectionName); 
}

void AInGamePlayer::S2C_StopAttackMontage_Implementation()
{
	StopAnimMontage(BasicComboAttackMontage); 
}

void AInGamePlayer::S2C_PlayRollingMontage_Implementation(FName SectionName)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		float MontageLength = PlayAnimMontage(RollingMontage, 1.0f, SectionName);
		if (MontageLength > 0)
		{
			FOnMontageEnded EndDelegate;
			EndDelegate.BindLambda([WeakThis = TWeakObjectPtr<AInGamePlayer>(this)](UAnimMontage* Montage, bool bInterrupted) {
				if (!bInterrupted && WeakThis.IsValid())
				{
					if (WeakThis->HasAuthority())
					{
						WeakThis->SetCurrentState(WeakThis->PrevState);
					}
				}
				});
			AnimInstance->Montage_SetEndDelegate(EndDelegate);
		}
	}
}

FName AInGamePlayer::GetRollingSectionName(float Direction)
{
	if (Direction >= -22.5f && Direction < 22.5f)   
		return FName("Forward");
	if (Direction >= 22.5f && Direction < 67.5f)    
		return FName("Forward_Right");
	if (Direction >= 67.5f && Direction < 112.5f)   
		return FName("Right");
	if (Direction >= 112.5f && Direction < 157.5f)                         
		return FName("Backward_Right");
	if (Direction >= -67.5f && Direction < -22.5f)  
		return FName("Forward_Left");
	if (Direction >= -112.5f && Direction < -67.5f) 
		return FName("Left");
	if (Direction >= -157.5f && Direction < -112.5f)
		return FName("Backward_Left");

	return FName("Backward");
}

void AInGamePlayer::BasicAttackTrace()
{
	if (HasAuthority())
	{
		UAttackFunction::BasicAttackTraceShot(DT_AttackData, AttackSectionName, this);
	}
}

float AInGamePlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (IsLocallyControlled())
	{
		UpDateUI();
	}

	return 0.0f;
}

void AInGamePlayer::UpDateUI()
{
	if (Controller)
	{
		Controller->CallRefreshPlayerStat(HP, MaxHP, Posture, MaxPosture);
	}
}

