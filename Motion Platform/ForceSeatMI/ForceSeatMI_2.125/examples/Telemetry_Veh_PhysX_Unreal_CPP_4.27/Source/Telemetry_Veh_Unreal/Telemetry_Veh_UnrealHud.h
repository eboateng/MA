// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "Telemetry_Veh_UnrealHud.generated.h"


UCLASS(config = Game)
class ATelemetry_Veh_UnrealHud : public AHUD
{
	GENERATED_BODY()

public:
	ATelemetry_Veh_UnrealHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
