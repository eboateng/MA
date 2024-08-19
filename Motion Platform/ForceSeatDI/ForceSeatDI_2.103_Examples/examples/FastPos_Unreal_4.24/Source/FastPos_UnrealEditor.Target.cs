using UnrealBuildTool;
using System.Collections.Generic;

public class FastPos_UnrealEditorTarget : TargetRules
{
	public FastPos_UnrealEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.AddRange( new string[] { "FastPos_Unreal" } );
	}
}
