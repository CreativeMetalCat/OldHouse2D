// Fill out your copyright notice in the Description page of Project Settings.


#include "PossesivePlayerController.h"

void APossesivePlayerController::BeginPlay()
{
    OriginalHost = GetPawn();
    
    FInputModeGameAndUI inputMode = FInputModeGameAndUI();
    inputMode.SetHideCursorDuringCapture(false);
    SetInputMode(FInputModeGameAndUI());

    bShowMouseCursor = true;
}
