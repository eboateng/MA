using UnrealBuildTool;
using System.Collections.Generic;

public class TablePhyPos_UnrealEditorTarget : TargetRules
{
	public TablePhyPos_UnrealEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.AddRange( new string[] { "TablePhyPos_Unreal" } );
	}
}
