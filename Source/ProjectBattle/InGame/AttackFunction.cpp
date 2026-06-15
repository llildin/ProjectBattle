// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/AttackFunction.h"
#include "InGame/Contents/Sturcture_AttackData.h"
#include "Kismet/KismetSystemLibrary.h"

void UAttackFunction::BasicAttackTraceShot(UDataTable* DT_AttackData, FString AttackSectionName, AActor* Actor)
{
	if (DT_AttackData == nullptr)
	{
		return;
	}
	
	FAttackData* AttackData = DT_AttackData->FindRow<FAttackData>(FName(AttackSectionName), TEXT(""));

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
		EDrawDebugTrace::ForDuration,
		OutHits,
		true
	);

}

FVector UAttackFunction::GetTraceLocation(FVector Offset, AActor* Actor)
{
	FVector TraceLocation = Actor->GetActorLocation() +
		Actor->GetActorForwardVector() * Offset.X +
		Actor->GetActorRightVector() * Offset.Y +
		Actor->GetActorUpVector() * Offset.Z;

	return TraceLocation;
}

