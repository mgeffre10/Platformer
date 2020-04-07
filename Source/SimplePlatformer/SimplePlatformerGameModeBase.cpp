// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "SimplePlatformerGameModeBase.h"
#include "PlayerCharacter.h"

ASimplePlatformerGameModeBase::ASimplePlatformerGameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
}