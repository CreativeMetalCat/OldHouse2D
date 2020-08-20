// Copyright Epic Games, Inc. All Rights Reserved.

#include "OldHouseGameMode.h"
#include "OldHouseCharacter.h"
#include "Player/PossesivePlayerController.h"

AOldHouseGameMode::AOldHouseGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AOldHouseCharacter::StaticClass();

	PlayerControllerClass = APossesivePlayerController::StaticClass();
}
