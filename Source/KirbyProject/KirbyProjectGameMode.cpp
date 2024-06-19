// Copyright Epic Games, Inc. All Rights Reserved.

#include "KirbyProjectGameMode.h"
#include "KirbyProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKirbyProjectGameMode::AKirbyProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
