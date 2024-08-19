// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Telemetry_Veh_UnrealGameMode.h"
#include "Telemetry_Veh_Unreal.h"
#include "Telemetry_Veh_UnrealPawn.h"
#include "Telemetry_Veh_UnrealHud.h"

ATelemetry_Veh_UnrealGameMode::ATelemetry_Veh_UnrealGameMode()
{
	DefaultPawnClass = ATelemetry_Veh_UnrealPawn::StaticClass();
	HUDClass = ATelemetry_Veh_UnrealHud::StaticClass();
}
