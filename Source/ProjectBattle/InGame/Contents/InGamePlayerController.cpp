// Fill out your copyright notice in the Description page of Project Settings.


#include "InGame/Contents/InGamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void AInGamePlayerController::OnPossess(APawn* aPawn)
{
    Super::OnPossess(aPawn);


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

