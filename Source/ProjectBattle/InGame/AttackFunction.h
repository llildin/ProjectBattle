// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AttackFunction.generated.h"

class AActor;

/**
 * 
 */
UCLASS()
class PROJECTBATTLE_API UAttackFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static void BasicAttackTraceShot(UDataTable* AttackData, FString AttackSectionName, AActor* Actor);

	static FVector GetTraceLocation(FVector Offset, AActor* Actor);
};
