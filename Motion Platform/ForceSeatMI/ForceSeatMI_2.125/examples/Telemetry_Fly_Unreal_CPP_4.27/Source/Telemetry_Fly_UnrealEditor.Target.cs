using UnrealBuildTool;
using System.Collections.Generic;

public class Telemetry_Fly_UnrealEditorTarget : TargetRules
{
	public Telemetry_Fly_UnrealEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("Telemetry_Fly_Unreal");
	}
}
