// Copyright Epic Games, Inc. All Rights Reserved.

#include "OldHouseGameMode.h"
#include "OldHouseCharacter.h"

AOldHouseGameMode::AOldHouseGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AOldHouseCharacter::StaticClass();	
}
