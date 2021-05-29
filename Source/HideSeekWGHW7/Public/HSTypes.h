// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "HSTypes.generated.h"


UENUM(BlueprintType)
enum class ETeamType : uint8
{
	Hide,
	Seek,
};

UENUM(BlueprintType)
enum class EHSGameStage : uint8
{
	Preparation,
	Hidding,
	Seeking,
	Over,
};
