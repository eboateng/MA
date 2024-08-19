using UnrealBuildTool;
using System.Collections.Generic;

public class TablePhyPos_UnrealTarget : TargetRules
{
	public TablePhyPos_UnrealTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.AddRange( new string[] { "TablePhyPos_Unreal" } );
	}
}
