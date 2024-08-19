// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Telemetry_Fly_UnrealGameMode.h"
#include "Telemetry_Fly_Unreal.h"
#include "Telemetry_Fly_UnrealPawn.h"

ATelemetry_Fly_UnrealGameMode::ATelemetry_Fly_UnrealGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = ATelemetry_Fly_UnrealPawn::StaticClass();
}
