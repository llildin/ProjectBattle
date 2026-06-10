// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Human.generated.h"

class UStaticMeshComponent;

UENUM(BlueprintType)
enum class NPC_CurrentState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),
	BasicAttack = 10 UMETA(DisplayName = "BasicAttack"),
	Guard = 20 UMETA(DisplayName = "Guard"),
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


};
