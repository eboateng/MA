using UnrealBuildTool;
using System.Collections.Generic;

public class Telemetry_Veh_UnrealTarget : TargetRules
{
	public Telemetry_Veh_UnrealTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("Telemetry_Veh_Unreal");
	}
}
