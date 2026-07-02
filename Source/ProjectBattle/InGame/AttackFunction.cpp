// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/AttackFunction.h"
#include "InGame/Contents/Sturcture_AttackData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UAttackFunction::BasicAttackTraceShot(UDataTable* DT_AttackData, FString AttackSectionName, AActor* Actor)
{
	if (!Actor->HasAuthority())
	{
		return;
	}

	if (Actor == nullptr || Actor->GetWorld() == nullptr)
	{
		return;
	}

	if (DT_AttackData == nullptr)
	{
		return;
	}
	
	FAttackData* AttackData = DT_AttackData->FindRow<FAttackData>(FName(AttackSectionName), TEXT(""));

	if (AttackData == nullptr)
	{
		return;
	}

	FVector StartLocation = GetTraceLocation(AttackData->StartOffset, Actor);
	FVector EndLocation = GetTraceLocation(AttackData->EndOffset, Actor);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<FHitResult> OutHits;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Actor);

	bool bTraceHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		Actor->GetWorld(),
		StartLocation,
		EndLocation,
		30.0f,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		OutHits,
		true
	);

	if (bTraceHit)
	{
		TArray<AActor*> AlreadyHitActors;

		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();

			if (HitActor && !AlreadyHitActors.Contains(HitActor))
			{
				AlreadyHitActors.Add(HitActor);

				UGameplayStatics::ApplyDamage(
					HitActor,                      // 데미지 받을 대상 (AActor*)
					AttackData->Damage,            // 데미지 수치
					Actor->GetInstigatorController(), // 공격을 지시한 컨트롤러
					Actor,                      // 공격자 본인 (Damage Causer)
					UDamageType::StaticClass()     // 데미지 타입 (기본)
				);
			}
		}
	}
}

FVector UAttackFunction::GetTraceLocation(FVector Offset, AActor* Actor)
{
	FVector TraceLocation = Actor->GetActorLocation() +
		Actor->GetActorForwardVector() * Offset.X +
		Actor->GetActorRightVector() * Offset.Y +
		Actor->GetActorUpVector() * Offset.Z;

	return TraceLocation;
}

