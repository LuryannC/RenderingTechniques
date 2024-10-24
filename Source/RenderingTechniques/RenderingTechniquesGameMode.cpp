// Copyright Epic Games, Inc. All Rights Reserved.

#include "RenderingTechniquesGameMode.h"
#include "RenderingTechniquesCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARenderingTechniquesGameMode::ARenderingTechniquesGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
