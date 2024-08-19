// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Telemetry_Veh_UnrealWheelFront.h"
#include "Telemetry_Veh_Unreal.h"

UTelemetry_Veh_UnrealWheelFront::UTelemetry_Veh_UnrealWheelFront()
{
	ShapeRadius = 35.f;
	ShapeWidth = 10.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 50.f;
}
