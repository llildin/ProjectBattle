// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "MyGameInstance.h"

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerState, Nickname);
}

void ALobbyPlayerState::CopyProperties(APlayerState* NewPlayerState)
{
}

void ALobbyPlayerState::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("[1] BeginPlay called. HasAuthority: %d"), HasAuthority());
    UE_LOG(LogTemp, Warning, TEXT("[2] PlayerController: %s"),
        GetPlayerController() ? TEXT("Valid") : TEXT("NULL"));

    if (GetPlayerController())
    {
        UE_LOG(LogTemp, Warning, TEXT("[3] IsLocalController: %d"),
            GetPlayerController()->IsLocalController());
    }

    if (GetPlayerController() && GetPlayerController()->IsLocalController())
    {
        UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
        UE_LOG(LogTemp, Warning, TEXT("[4] GameInstance: %s, Nickname: %s"),
            GI ? TEXT("Valid") : TEXT("NULL"),
            GI ? *GI->Nickname : TEXT("N/A"));

        if (GI)
        {
            C2S_SetNickname(GI->Nickname);
            UE_LOG(LogTemp, Warning, TEXT("[5] C2S_SetNickname called"));
        }
    }
}

bool ALobbyPlayerState::C2S_SetNickname_Validate(const FString& InName)
{
    return true;
}

void ALobbyPlayerState::C2S_SetNickname_Implementation(const FString& InName)
{
    UE_LOG(LogTemp, Warning, TEXT("[6] C2S_SetNickname_Implementation called with: %s"), *InName);
    Nickname = InName;
    UE_LOG(LogTemp, Warning, TEXT("[7] Nickname set to: %s"), *Nickname);
}