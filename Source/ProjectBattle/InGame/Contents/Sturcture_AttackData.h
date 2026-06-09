#pragma once
#include "CoreMinimal.h"
#include "Sturcture_AttackData.generated.h" 

USTRUCT(BlueprintType)
struct FAttackData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    FVector StartOffset;

    UPROPERTY(EditAnywhere)
    FVector EndOffset;

    UPROPERTY(EditAnywhere)
    float Damage;
};