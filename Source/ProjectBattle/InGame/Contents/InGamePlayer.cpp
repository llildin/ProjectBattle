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


	// 연계 공격 버튼을 눌러 회전이 필요한 상태일 때만 작동
	if (bIsComboRotating)
	{
		FRotator CurrentRot = GetActorRotation();

		// 1타 회전값(현재) -> 2타 회전값(목표)으로 부드럽게 보간
		FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetAttackRotation, DeltaTime, ComboRotationSpeed);
		SetActorRotation(NewRot);

		// 목표치에 거의 도달했다면 틱 회전을 멈춰서 최적화 (오차범위 1도 이내)
		if (CurrentRot.Equals(TargetAttackRotation, 1.0f))
		{
			bIsComboRotating = false;
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
		// 1. 공격 입력이 들어온 '그 순간'의 카메라(컨트롤러) Yaw값 저장
		FRotator CameraRot = GetControlRotation();
		TargetAttackRotation = FRotator(0.f, CameraRot.Yaw, 0.f);

		// 2. 부드러운 회전을 방해하는 기본 카메라 동기화 옵션을 잠시 끕니다.
		bUseControllerRotationYaw = false;

		// 3. 틱에서 회전 로직을 가동하도록 플래그 오픈
		bIsComboRotating = true;

		// 4. 다음 공격 몽타주 재생 로직...
		// PlayAnimMontage(AttackMontage_2);

		// 5. [중요] 몽타주가 끝났을 때 다시 카메라 동기화를 켜주기 위해 델리게이트 등록
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &AInGamePlayer::OnAttackMontageEnded);
			AnimInstance->OnMontageEnded.AddDynamic(this, &AInGamePlayer::OnAttackMontageEnded);
		}



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

void AInGamePlayer::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 공격이 끝나면 다시 평소처럼 카메라 방향을 즉시 바라보도록 복구
	bUseControllerRotationYaw = true;
	bIsComboRotating = false;

	// 메모리 누수 방지를 위해 사용한 델리게이트 해제
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &AInGamePlayer::OnAttackMontageEnded);
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

