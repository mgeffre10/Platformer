// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "SimplePlatformerGameModeBase.h"
#include "PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASimplePlatformerGameModeBase::ASimplePlatformerGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/PlayerCharacter_BP"));
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/DIY/MyDIY_PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}