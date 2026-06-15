// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Human.generated.h"

class UStaticMeshComponent;

UENUM(BlueprintType)
enum class ECurrentState : uint8
{
	No_Battle = 0 UMETA(DisplayName = "No_Battle"),
	Battle = 10 UMETA(DisplayName = "Battle"),
	Guard = 20 UMETA(DisplayName = "Guard"),
	Attack = 30 UMETA(DisplayName = "Attack"),
	BasicAttack = 31 UMETA(DisplayName = "BasicAttack"),
	On_Damaged = 40 UMETA(DisplayName = "On_Damaged"),
	Rolling = 50 UMETA(DisplayName = "Rolling"),
	Interact = 60 UMETA(DisplayName = "Interact")
};

UCLASS()
class PROJECTBATTLE_API AHuman : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHuman();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Katana;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	uint8 Hp = 100;
};
