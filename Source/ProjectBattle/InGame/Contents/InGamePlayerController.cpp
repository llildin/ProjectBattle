// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/InGamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "NPCSetting.h"
#include "InGameBaseUI.h"
#include "InGamePlayer.h"

void AInGamePlayerController::AcknowledgePossession(APawn* aPawn)
{
    Super::AcknowledgePossession(aPawn);

    
    if (IsLocalPlayerController())
    {
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

        if (Subsystem)
        {
            Subsystem->ClearAllMappings();
            for (const auto& IMC : IMC_InGame)
            {
                Subsystem->AddMappingContext(IMC.LoadSynchronous(), 0);
            }
        }

        Player = Cast<AInGamePlayer>(aPawn);
    }
}

void AInGamePlayerController::OnUnPossess()
{
    Super::OnUnPossess();

    if (IsLocalPlayerController())
    {
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

        if (Subsystem)
        {
            Subsystem->ClearAllMappings();
        }
    }
}

void AInGamePlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalPlayerController())
    {
        InGameBaseUIObject = CreateWidget<UInGameBaseUI>(this, InGameBaseUIClass);
        InGameBaseUIObject->AddToViewport();
        InGameBaseUIObject->SetVisibility(ESlateVisibility::Visible);

        NPCSettingObject = CreateWidget<UNPCSetting>(this, NPCSettingClass);
        NPCSettingObject->AddToViewport();
        NPCSettingObject->SetVisibility(ESlateVisibility::Collapsed);

        bShowMouseCursor = false;
        SetInputMode(FInputModeGameOnly());

        NPCSettingObject->OnCloseUI.BindLambda([this]()
            {
                InGameBaseUIObject->SetVisibility(ESlateVisibility::Visible);
                NPCSettingObject->SetVisibility(ESlateVisibility::Collapsed);

                bShowMouseCursor = false;
                SetInputMode(FInputModeGameOnly());

                if (IsLocalPlayerController())
                {
                    Player->SetCurrentState(Player->PrevState);
                }
            });
    }
}

void AInGamePlayerController::NPCSettingInteract()
{
    InGameBaseUIObject->SetVisibility(ESlateVisibility::Collapsed);
    NPCSettingObject->SetVisibility(ESlateVisibility::Visible);

    Player->C2S_SetCurrentState(ECurrentState::Interact);

    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());
}

void AInGamePlayerController::CallRefreshPlayerStat(float NewHp, float MaxHp, float NewPosture, float MaxPosture)
{
    InGameBaseUIObject->RefreshPlayerStat(NewHp, MaxHp, NewPosture, MaxPosture);
}
