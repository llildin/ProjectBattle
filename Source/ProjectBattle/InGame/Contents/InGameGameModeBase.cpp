// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/InGameGameModeBase.h"
#include "InGamePlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"

void AInGameGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    FindPlayerStarts();
}

void AInGameGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    AInGamePlayerState* PS =
        NewPlayer->GetPlayerState<AInGamePlayerState>();

    if (!PS)
    {
        return;
    }

    if (PS->IsPlayer)
    {
        RestartPlayer(NewPlayer); 
    }
    else
    {
        NewPlayer->StartSpectatingOnly();
    }

    if (PS->IsPlayer)
    {
        PlayingControllers.Add(NewPlayer);

        RestartPlayer(NewPlayer);

        if (PlayingControllers.Num() == 2)
        {
            InitSpectators();
        }
    }
    else
    {
        SpectatorControllers.Add(NewPlayer);

        NewPlayer->StartSpectatingOnly();
    }
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
        else if (Start->ActorHasTag("PlayerStart3"))
        {
            PlayerStart3 = Start;
        }
    }
}

void AInGameGameModeBase::InitSpectators()
{
    if (PlayingControllers.Num() < 2)
        return;

    APawn* TargetPawn = PlayingControllers[0]->GetPawn();

    if (!TargetPawn)
        return;

    for (APlayerController* PC : SpectatorControllers)
    {
        if (!PC)
            continue;

        PC->SetViewTargetWithBlend(TargetPawn, 0.f);
    }
}

AActor* AInGameGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
    if (PlayingControllers.Num() == 0)
    {
        return Super::ChoosePlayerStart_Implementation(Player);
    }

    if (PlayingControllers.IsValidIndex(0))
    {
        if (PlayingControllers[0] == Player)
        {
            return PlayerStart1;
        }
    }

    if (PlayingControllers.IsValidIndex(1))
    {
        if (PlayingControllers[1] == Player)
        {
            return PlayerStart2;
        }
    }

    return PlayerStart3;
}

void AInGameGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    Super::HandleStartingNewPlayer_Implementation(NewPlayer);

    AInGamePlayerState* PS =
        NewPlayer->GetPlayerState<AInGamePlayerState>();

    if (!PS)
    {
        return;
    }

    if (PS->IsPlayer)
    {
        RestartPlayer(NewPlayer);
    }
    else
    {
        NewPlayer->StartSpectatingOnly();
    }

    if (PS->IsPlayer)
    {
        PlayingControllers.Add(NewPlayer);

        RestartPlayer(NewPlayer);

        if (PlayingControllers.Num() == 2)
        {
            InitSpectators();
        }
    }
    else
    {
        SpectatorControllers.Add(NewPlayer);

        NewPlayer->StartSpectatingOnly();
    }
}

