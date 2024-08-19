// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class MotionCueingInterface : ModuleRules
{
	public MotionCueingInterface(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "Engine", "ForceSeatMI" });
		PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine", "Slate", "SlateCore" });
	}
}
