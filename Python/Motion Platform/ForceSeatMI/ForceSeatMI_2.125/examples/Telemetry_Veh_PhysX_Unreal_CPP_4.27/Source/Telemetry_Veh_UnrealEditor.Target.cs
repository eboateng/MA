using UnrealBuildTool;
using System.Collections.Generic;

public class Telemetry_Veh_UnrealEditorTarget : TargetRules
{
	public Telemetry_Veh_UnrealEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("Telemetry_Veh_Unreal");
	}
}
