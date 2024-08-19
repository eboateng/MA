using UnrealBuildTool;
using System.Collections.Generic;

public class Telemetry_Fly_UnrealTarget : TargetRules
{
	public Telemetry_Fly_UnrealTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Telemetry_Fly_Unreal");
	}
}
