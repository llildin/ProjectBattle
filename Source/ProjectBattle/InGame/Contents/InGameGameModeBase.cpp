// InGameGameModeBase.cpp
// Fill out your copyright notice in the Description page of Project Settings.

#include "InGame/Contents/InGameGameModeBase.h"
#include "InGamePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"

void AInGameGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    FindPlayerStarts();
}

void AInGameGameModeBase::FindPlayerStarts()
{
    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        APlayerStart::StaticClass(),
        Found);

    for (AActor* Actor : Found)
    {
        APlayerStart* Start = Cast<APlayerStart>(Actor);
        if (!Start)
            continue;

        if (Start->ActorHasTag("PlayerStart1"))
        {
            PlayerStart1 = Start;
        }
        else if (Start->ActorHasTag("PlayerStart2"))
        {
            PlayerStart2 = Start;
        }
    }
}

AActor* AInGameGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
    if (PlayingControllers.IsValidIndex(0) && PlayingControllers[0] == Player)
    {
        return PlayerStart1;
    }
    if (PlayingControllers.IsValidIndex(1) && PlayingControllers[1] == Player)
    {
        return PlayerStart2;
    }

    return Super::ChoosePlayerStart_Implementation(Player);
}

void AInGameGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    AInGamePlayerState* PS = NewPlayer->GetPlayerState<AInGamePlayerState>();
    if (!PS)
    {
        Super::HandleStartingNewPlayer_Implementation(NewPlayer);
        return;
    }

    if (PS->IsPlayer)
    {
        PlayingControllers.Add(NewPlayer);

        Super::HandleStartingNewPlayer_Implementation(NewPlayer);

        RefreshSpectatorViewTargets();
    }
    else
    {
        SpectatorControllers.Add(NewPlayer);

        NewPlayer->StartSpectatingOnly();

        RefreshSpectatorViewTargets();
    }
}

void AInGameGameModeBase::RefreshSpectatorViewTargets()
{
    if (PlayingControllers.Num() == 0 || !PlayingControllers[0])
    {
        return;
    }

    APawn* TargetPawn = PlayingControllers[0]->GetPawn();
    if (!TargetPawn)
    {
        return;
    }

    for (APlayerController* PC : SpectatorControllers)
    {
        if (PC)
        {
            PC->SetViewTargetWithBlend(TargetPawn, 0.f);
        }
    }
}