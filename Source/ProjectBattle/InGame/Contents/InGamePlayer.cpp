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

	Controller = Cast<AInGamePlayerController>(GetController());
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

		UIC->BindAction(IA_Guard, ETriggerEvent::Started, this, &AInGamePlayer::GuardStart);
		UIC->BindAction(IA_Guard, ETriggerEvent::Completed, this, &AInGamePlayer::GuardEnd);

		UIC->BindAction(IA_Roll, ETriggerEvent::Started, this, &AInGamePlayer::Roll);
		UIC->BindAction(IA_Run, ETriggerEvent::Started, this, &AInGamePlayer::RunStart);
		UIC->BindAction(IA_Run, ETriggerEvent::Completed, this, &AInGamePlayer::RunEnd);

		UIC->BindAction(IA_Interact, ETriggerEvent::Started, this, &AInGamePlayer::Interact);
	}
}

void AInGamePlayer::Move(const FInputActionValue& Value)
{
	if (CurrentState != ECurrentState::Attack && CurrentState != ECurrentState::On_Damaged && CurrentState != ECurrentState::Rolling)
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
		SetCurrentState(ECurrentState::No_Battle);
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AInGamePlayer::GuardStart(const FInputActionValue& Value)
{
	if (CurrentState == ECurrentState::Battle)
	{
		SetCurrentState(ECurrentState::Guard);
	}
}

void AInGamePlayer::GuardEnd(const FInputActionValue& Value)
{
	if (CurrentState == ECurrentState::Guard)
	{
		SetCurrentState(ECurrentState::Battle);
	}
}

void AInGamePlayer::Roll(const FInputActionValue& Value)
{
	if (CurrentState == ECurrentState::No_Battle)
	{
		PrevState = CurrentState;
		SetCurrentState(ECurrentState::Rolling);
		Rolling();
	}
	else if (CurrentState != ECurrentState::Rolling && CurrentState != ECurrentState::On_Damaged)
	{
		PrevState = ECurrentState::Battle;
		SetCurrentState(ECurrentState::Rolling);
		Rolling();
	}
}

void AInGamePlayer::RunStart(const FInputActionValue& Value)
{
	CurrentMoveState = EMoveState::Run;
	UpdateMoveSpeed();
}

void AInGamePlayer::RunEnd(const FInputActionValue& Value)
{
	CurrentMoveState = EMoveState::Idle;
	UpdateMoveSpeed();
}

void AInGamePlayer::Interact(const FInputActionValue& Value)
{
	if (bIsNPCSetting)
	{
		Controller->NPCSettingInteract();

		GetMovementComponent()->StopMovementImmediately();
	}
}

void AInGamePlayer::SetCurrentState(ECurrentState NewState)
{
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
	if (PlayingBasicComboAttackIndex != BasicComboAttackCount)
	{
		PlayBasicComboAttackMontage();
		PlayingBasicComboAttackIndex = BasicComboAttackCount;
	}
}

void AInGamePlayer::BasicComboAttack()
{
	if (CurrentState == ECurrentState::No_Battle || CurrentState == ECurrentState::Battle 
		|| CurrentState == ECurrentState::Attack || CurrentState == ECurrentState::Guard)
	{
		if (!bIsBasicAttacking)
		{
			BasicComboAttackCount++;

			PlayBasicComboAttackMontage();

			bIsBasicAttacking = true;

			if (CurrentState == ECurrentState::No_Battle)
			{
				GetCharacterMovement()->bOrientRotationToMovement = false;

				FRotator StartRotator = GetControlRotation();
				FRotator EndRotator = GetActorRotation();
				EndRotator.Pitch = -40.0f;
				EndRotator.Roll = 0.0f;
				BattleCameraSetting(StartRotator, EndRotator);
			}

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
		AttackSectionName = FString::Printf(TEXT("BasicAttack0%d"), BasicComboAttackCount);

		float MontageLength = PlayAnimMontage(BasicComboAttackMontage, 1.0f, FName(AttackSectionName));

		if (MontageLength > 0)
		{
			FOnMontageEnded EndDelegate;

			EndDelegate.BindLambda([WeakThis = TWeakObjectPtr<AInGamePlayer>(this)](UAnimMontage* Montage, bool bInterrupted) {
				if (!bInterrupted && WeakThis.IsValid())
				{
					WeakThis->BasicComboAttackCount = 0;
					WeakThis->PlayingBasicComboAttackIndex = 0;
					WeakThis->bIsBasicAttacking = false;
					WeakThis->SetCurrentState(ECurrentState::Battle);
				}
				});
			AnimInstance->Montage_SetEndDelegate(EndDelegate);
		}
	}
}

void AInGamePlayer::Rolling()
{
	if (bIsBasicAttacking)
	{
		StopAnimMontage(BasicComboAttackMontage);
		BasicComboAttackCount = 0;
		PlayingBasicComboAttackIndex = 0;
		bIsBasicAttacking = false;
	}

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{		
		float Direction = UKismetAnimationLibrary::CalculateDirection(LastInputVector, GetActorRotation());

		FName SectionName = GetRollingSectionName(Direction);
		float MontageLength = PlayAnimMontage(RollingMontage, 1.0f, SectionName);

		if (MontageLength > 0)
		{
			FOnMontageEnded EndDelegate;

			EndDelegate.BindLambda([WeakThis = TWeakObjectPtr<AInGamePlayer>(this)](UAnimMontage* Montage, bool bInterrupted) {
				if (!bInterrupted && WeakThis.IsValid())
				{
					WeakThis->SetCurrentState(WeakThis->PrevState);
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
	if (DT_AttackData == nullptr)
	{
		return;
	}

	FAttackData* AttackData = DT_AttackData->FindRow<FAttackData>(FName(AttackSectionName), TEXT(""));

	FVector StartLocation = GetTraceLocation(AttackData->StartOffset);
	FVector EndLocation = GetTraceLocation(AttackData->EndOffset);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<FHitResult> OutHits;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	bool bTraceHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		StartLocation,
		EndLocation,
		30.0f,
		ObjectTypes,
		false,      
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHits,
		true                   
	);

}

FVector AInGamePlayer::GetTraceLocation(FVector Offset)
{
	FVector TraceLocation = GetActorLocation() +
		GetActorForwardVector() * Offset.X +
		GetActorRightVector() * Offset.Y +
		GetActorUpVector() * Offset.Z;

	return TraceLocation;
}

