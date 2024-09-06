// Copyright Epic Games, Inc. All Rights Reserved.

#include "Again30GameMode.h"
#include "Again30Character.h"
#include "UObject/ConstructorHelpers.h"

AAgain30GameMode::AAgain30GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
