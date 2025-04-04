// Copyright Epic Games, Inc. All Rights Reserved.

#include "DylanAdventureGameMode.h"
#include "DylanAdventureCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADylanAdventureGameMode::ADylanAdventureGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
