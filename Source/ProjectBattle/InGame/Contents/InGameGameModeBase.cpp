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
    // 참여자 목록에서 몇 번째인지에 따라 PlayerStart1 / PlayerStart2로 배치
    if (PlayingControllers.IsValidIndex(0) && PlayingControllers[0] == Player)
    {
        return PlayerStart1;
    }
    if (PlayingControllers.IsValidIndex(1) && PlayingControllers[1] == Player)
    {
        return PlayerStart2;
    }

    // 이론상 여기 도달하면 안 되지만(관전자는 RestartPlayer 자체를 안 타므로), 안전망으로 기본 동작
    return Super::ChoosePlayerStart_Implementation(Player);
}

void AInGameGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    AInGamePlayerState* PS = NewPlayer->GetPlayerState<AInGamePlayerState>();
    if (!PS)
    {
        // PlayerState가 없는 예외적인 경우 기본 동작으로 폴백
        Super::HandleStartingNewPlayer_Implementation(NewPlayer);
        return;
    }

    if (PS->IsPlayer)
    {
        PlayingControllers.Add(NewPlayer);

        // 여기서 ChoosePlayerStart_Implementation → RestartPlayer 순서로
        // 실제 Pawn 스폰이 이루어짐 (PlayerStart1/2 사용)
        Super::HandleStartingNewPlayer_Implementation(NewPlayer);

        // 참여자 Pawn이 새로 생겼으니, 기존 관전자들 시점도 갱신
        RefreshSpectatorViewTargets();
    }
    else
    {
        SpectatorControllers.Add(NewPlayer);

        // Pawn을 아예 스폰하지 않고 순수 관전 모드로 전환
        // (Super::HandleStartingNewPlayer_Implementation을 호출하지 않으므로
        //  RestartPlayer/ChoosePlayerStart가 실행되지 않음 = Pawn 생성 없음)
        NewPlayer->StartSpectatingOnly();

        // 이미 참여자가 있다면 즉시 그 시점으로 연결
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